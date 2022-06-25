import json
import os

from selenium import webdriver


# chrome反爬虫设置
# headless:True 不显示浏览器窗口
def create_chrome_driver(*, headless=False):
    options = webdriver.ChromeOptions()
    if headless:
        options.add_argument("--headless")
    options.add_experimental_option("excludeSwitches", ["enable-automation"])
    options.add_experimental_option("useAutomationExtension", False)
    b = webdriver.Chrome(options=options)
    b.execute_cdp_cmd(
        "Page.addScriptToEvaluateOnNewDocument",
        {
            "source": 'Object.defineProperty(navigator, "webdriver", {get: ()=>undefined})'
        },
    )
    return b


def add_cookies(b, cookie_file):
    with open(cookie_file, "r") as f:
        cookies_list = json.load(f)
        for cookie_dict in cookies_list:
            if cookie_dict["secure"]:
                b.add_cookie(cookie_dict)
