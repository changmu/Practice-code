package main

import (
	"fmt"
	"regexp"
	"strings"
	"time"

	"github.com/gocolly/colly/debug"
	log "github.com/sirupsen/logrus"

	"github.com/PuerkitoBio/goquery"
	"github.com/gocolly/colly"
	"github.com/gocolly/colly/extensions"
)

func TaskDoubanMovie() {
	t := time.Now()
	number := 1

	c := colly.NewCollector(
		colly.Async(true),
		colly.Debugger(&debug.LogDebugger{}),
		//过滤url,去除不是https://movie.douban.com/top250?start=0&filter= 的url
		colly.URLFilters(
			regexp.MustCompile("^(https://movie\\.douban\\.com/top250)\\?start=[0-9].*&filter="),
		),
	)
	extensions.RandomUserAgent(c) // 设置随机头，用于反爬虫

	// 配置限速
	err := c.Limit(&colly.LimitRule{
		DomainGlob:  "*",
		Parallelism: 1,
		Delay:       2 * time.Second,
	})
	if err != nil {
		log.Fatalf("c.Limit err:%v", err)
	}

	log.Debugf("hello")
	// 创建收集器
	c.OnRequest(func(r *colly.Request) {
		r.Headers.Set("Cookie", "bid=mslRL-D9ODc; gr_user_id=5a0d1bc6-0a91-4a54-bd2d-85bb99d18801; __utmc=30149280; viewed=\"27117578_35640714_23018929_27019116_3578243_25917721\"; dbcl2=\"80395974:n6KMCYOS8W4\"; ck=QVxL; _pk_ref.100001.4cf6=[\"\",\"\",1655884902,\"https://accounts.douban.com/\"]; _pk_ses.100001.4cf6=*; __utma=30149280.144366142.1653660529.1655816371.1655884908.5; __utmb=30149280.0.10.1655884908; __utmz=30149280.1655884908.5.5.utmcsr=accounts.douban.com|utmccn=(referral)|utmcmd=referral|utmcct=/; __utma=223695111.900526156.1655884908.1655884908.1655884908.1; __utmb=223695111.0.10.1655884908; __utmc=223695111; __utmz=223695111.1655884908.1.1.utmcsr=accounts.douban.com|utmccn=(referral)|utmcmd=referral|utmcct=/; push_noty_num=0; push_doumail_num=0; _pk_id.100001.4cf6=525229b9e0a75c97.1655884902.1.1655884920.1655884902.")
	})
	// 响应的格式为HTML,提取页面中的链接
	c.OnHTML("a[href]", func(e *colly.HTMLElement) {
		link := e.Attr("href")
		//fmt.Printf("find link: %s\n", e.Request.AbsoluteURL(link))
		c.Visit(e.Request.AbsoluteURL(link))
	})
	// 获取电影信息
	c.OnHTML("div.info", func(e *colly.HTMLElement) {
		e.DOM.Each(func(i int, selection *goquery.Selection) {
			movies := selection.Find("span.title").First().Text()
			director := strings.Join(strings.Fields(selection.Find("div.bd p").First().Text()), " ")
			quote := selection.Find("p.quote span.inq").Text()
			log.Debugf("%d --> %s:%s %s", number, movies, director, quote)
			number += 1
		})
	})
	c.OnError(func(response *colly.Response, err error) {
		fmt.Println(err)
	})
	err = c.Visit("https://movie.douban.com/top250?start=0&filter=")
	if err != nil {
		log.Fatalf("c.Visit err:%v", err)
	}
	c.Wait()
	fmt.Printf("花费时间:%s", time.Since(t))
}
