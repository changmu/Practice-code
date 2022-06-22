package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/url"
	"os"
	"strings"
	"time"

	"github.com/gocolly/colly"
	"github.com/gocolly/colly/extensions"
)

/*
   1.先F12,观察https://www.quanjing.com/search.aspx?q=%E5%8D%A1%E9%80%9A#%E5%8D%A1%E9%80%9A||1|1000|3|2||||||这个网站
   2.会发现其实这是一个通过json来加载数据的！通过这个URL:https://www.quanjing.com/Handler/SearchUrl.ashx
   3. 我们在看看都传递了哪些参数，照着来就好啦。
*/
// TaskQuanjingPic 下载图片
func TaskQuanjingPic() {
	t := time.Now()
	c := colly.NewCollector(func(collector *colly.Collector) {
		collector.Async = true
		extensions.RandomUserAgent(collector)
	})
	imageC := c.Clone()
	// 请求头
	c.OnRequest(func(r *colly.Request) {
		r.Headers.Set("Cookie", "BIGipServerPools_Web_ssl=3226249408.47873.0000; Hm_lvt_c01558ab05fd344e898880e9fc1b65c4=1655898565; qimo_seosource_578c8dc0-6fab-11e8-ab7a-fda8d0606763=绔欏唴; qimo_seokeywords_578c8dc0-6fab-11e8-ab7a-fda8d0606763=; accessId=578c8dc0-6fab-11e8-ab7a-fda8d0606763; ASP.NET_SessionId=3seouosp0qsug30qawk5s40i; validateCode=e869010730f979d3fd6d648428f87b96; pageViewNum=5; Hm_lpvt_c01558ab05fd344e898880e9fc1b65c4=1655898908")
		r.Headers.Add("referer", "https://www.quanjing.com/search.aspx?q=%E5%8D%A1%E9%80%9A")
		r.Headers.Add("sec-fetch-mode", "cors")
		r.Headers.Add("sec-fetch-site", "same-origin")
		r.Headers.Add("accept", "text/javascript, application/javascript, application/ecmascript, application/x-ecmascript, */*; q=0.01")
		r.Headers.Add("accept-encoding", "gzip, deflate, br")
		r.Headers.Add("accept-language", "en,zh-CN;q=0.9,zh;q=0.8")
		r.Headers.Add("X-Requested-With", "XMLHttpRequest")
	})
	//构造图片url，让图片imageC收集器来下载图片
	c.OnResponse(func(r *colly.Response) {
		var f interface{}
		if err := json.Unmarshal(r.Body[13:len(r.Body)-1], &f); err != nil {
			panic(err)
		}
		imgList := f.(map[string]interface{})["imglist"]
		for k, img := range imgList.([]interface{}) {
			url := img.(map[string]interface{})["imgurl"].(string)
			url = url + "#" + img.(map[string]interface{})["caption"].(string)
			fmt.Printf("find -->%d:%s\n", k, url)
			imageC.Visit(url)
		}
	})
	c.OnError(func(response *colly.Response, err error) {
		fmt.Println(err)
	})
	// 根据图片url来下载图片
	imageC.OnResponse(func(r *colly.Response) {
		fileName := ""
		caption := strings.Split(r.Request.URL.String(), "#") // 获得刚刚#后面的信息
		if len(caption) >= 2 {                                //这里要判断下没有信息的情况，要不然切片会越界
			fileName = caption[1] + ".jpg"
		} else {
			fileName = "未知"
		}
		res, err := url.QueryUnescape(fileName)       // 对url格式进行转换，要不然看不懂
		fileName = strings.Replace(res, ",", "_", -1) // 把信息中的逗号全部换成下换线，逗号文件命名会报错。
		fmt.Printf("下载 -->%s \n", fileName)
		f, err := os.Create("./download/" + fileName)
		if err != nil {
			panic(err)
		}
		io.Copy(f, bytes.NewReader(r.Body))
	})
	//构造URL
	pageSize := 200 //需要下载图片的数量,
	pageNum := 10
	for i := 0; i < pageNum; i++ {
		url := fmt.Sprintf("https://www.quanjing.com/Handler/SearchUrl.ashx?t=1952&callback=searchresult&q=卡通A&stype=1&pagesize=%d&pagenum=%d&imageType=2&imageColor=&brand=&imageSType=&fr=1&sortFlag=1&imageUType=&btype=&authid=&_=1577435470818", pageSize, i)
		_ = c.Visit(url)
	}

	c.Wait()
	imageC.Wait()
	fmt.Printf("done,cost:%s\n", time.Since(t))
}
