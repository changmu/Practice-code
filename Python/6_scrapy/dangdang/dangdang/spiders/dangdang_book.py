import scrapy
from scrapy_playwright.page import PageMethod


def join_strs_with_strip(ori_strs, sep=""):
    list = []
    for item in ori_strs:
        list.append(item.strip())
    return sep.join(list)


def extract_text_with_css(item, query):
    return item.css(query).get(default="").strip()


class DangdangBookSpider(scrapy.Spider):
    name = "dangdang_book"
    allowed_domains = ["dangdang.com"]

    def start_requests(self):
        for i in range(25):
            url = (
                "http://bang.dangdang.com/books/bestsellers/01.00.00.00.00.00-year-2021-0-1-"
                + str(i + 1)
            )
            yield scrapy.Request(
                url,
                meta={
                    "playwright": True,
                    "playwright_include_page": True,
                    "playwright_page_methods": [
                        PageMethod("wait_for_selector", "ul.bang_list"),  # 等待排名加载出来
                    ],
                    "errback": self.errback,
                },
            )

    async def errback(self, failure):
        self.logger.error("errback called, failure:%s", failure)
        page = failure.request.meta["playwright_page"]
        await page.close()

    async def parse(self, response):
        page = response.meta["playwright_page"]
        await page.close()
        for book in response.css("ul.bang_list>li"):
            # self.crawler.stats.inc_value("rank")
            item = {
                # "rank": self.crawler.stats.get_value("rank"),
                "rank": book.css("div.list_num::text").re_first(r"(\d+)\."),
                "title": book.css("div.name>a::attr(title)").get(default="").strip(),
                "author": book.css("div.publisher_info>a::attr(title)")
                .get(default="")
                .strip(),
                "comments": book.css("div.star>a::text").get(default="").strip(),
                "price": book.css("div.price span.price_n::text")
                .get(default="")
                .strip(),
            }
            yield item
            # yield scrapy.Request(
            #     book.css("div.name>a::attr(href)").get(default="").strip(),
            #     meta={
            #         "playwright": True,
            #         "playwright_include_page": True,
            #         "playwright_page_methods": [
            #             PageMethod("wait_for_selector", "#content"),  # 等内容简介加载出来
            #         ],
            #         "errback": self.errback,
            #     },
            #     cb_kwargs={"item": item},
            #     callback=self.parse_detail,
            # )

    async def parse_detail(self, response, item):
        page = response.meta["playwright_page"]
        await page.close()
        item["recommend_reason"] = extract_text_with_css(
            response, ".head_title_name::text"
        )
        item["content_intro"] = join_strs_with_strip(
            response.css("#content>.descrip ::text").getall()
        )
        item["author_introduction"] = join_strs_with_strip(
            response.css("#authorIntroduction>.descrip ::text").getall()
        )
        item["mediaFeedback"] = join_strs_with_strip(
            response.css("#mediaFeedback>.descrip ::text").getall()
        )
        item["content"] = join_strs_with_strip(
            response.css("#extract-show ::text").getall()
        )
        if item["content"] == "":
            item["content"] = join_strs_with_strip(
                response.css("#preface-show ::text").getall()
            )
        yield item
