import requests
import re


# 访问httpbin.org以调试请求头信息
def testRequests():
    # 构造请求头
    headers = {
        "User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36",
    }
    # 构造query参数
    params = {
        "a": 123,
        "b": "abc",
        "name": "秋牧Q",
    }
    # 发起get请求
    r = requests.get("https://httpbin.org/get", headers=headers, params=params)
    print(r.text)
    print("testRequests get done")

    # 构造表单数据
    data = {
        "data1": 123,
        "data2": "xyz",
    }
    # 构造文件
    files = {"file": open("favicon.ico", "rb")}
    r = requests.post(
        "https://httpbin.org/post",
        headers=headers,
        params=params,
        data=data,
        files=files,
    )
    print(r.text)
    print("testRequests post done")

def testBasicAuth():



def getPic():
    r = requests.get("https://scrape.center/favicon.ico")
    with open("favicon.ico", "wb") as f:
        f.write(r.content)
    print("download favicon.ico done")


def testRe():
    # 发起请求
    r = requests.get("https://ssr1.scrape.center/")
    # 构造正则
    pattern = re.compile("<h2.*?>(.*?)</h2>", re.S)
    # 提取数据
    titles = re.findall(pattern, r.text)
    print(titles)


if __name__ == "__main__":
    testBasicAuth()
