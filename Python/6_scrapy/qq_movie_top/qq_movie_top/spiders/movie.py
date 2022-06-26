import scrapy
from scrapy_playwright.page import PageMethod


class MovieSpider(scrapy.Spider):
    name = "movie"
    allowed_domains = ["v.qq.com"]

    def start_requests(self):
        url = "https://v.qq.com/channel/movie?channel=movie&itype=100004&listpage=1&sort=21"
        yield scrapy.Request(
            url,
            meta={
                "playwright": True,
                "playwright_include_page": True,
                "playwright_page_methods": [
                    PageMethod("wait_for_selector", "div.list_item"),
                    PageMethod(
                        "evaluate", "window.scrollBy(0, document.body.scrollHeight)"
                    ),
                    PageMethod(
                        "wait_for_selector", "div.list_item:nth-child(50)"
                    ),  # 30 per page
                ],
                "errback": self.errback,
            },
        )

    async def errback(self, failure):
        page = failure.request.meta["playwright_page"]
        await page.close()

    async def parse(self, response):
        page = response.meta["playwright_page"]
        # 截图保存
        # await page.screenshot(path="./example.png", full_page=True)
        await page.close()
        for movie in response.css("div.list_item"):
            self.crawler.stats.inc_value("rank")
            yield {
                "rank": self.crawler.stats.get_value("rank"),
                "title": movie.css("a.figure_title::text").get(default="").strip(),
                "author": movie.css("div.figure_desc::text").get(default="").strip(),
                "length": movie.css("div.figure_caption::text").get(default="").strip(),
                "grade": movie.css("div.figure_score::text").get(default="").strip(),
            }
