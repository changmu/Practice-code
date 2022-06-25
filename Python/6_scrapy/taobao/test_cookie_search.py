from util import create_chrome_driver, add_cookies

if __name__ == "__main__":
    b = create_chrome_driver()
    # 访问首页填充必要的header
    b.get("https://www.taobao.com")
    # 设置cookie
    add_cookies(b, "cook.json")
    # 开始搜索
    b.get("https://s.taobao.com/search?q=mbp")
