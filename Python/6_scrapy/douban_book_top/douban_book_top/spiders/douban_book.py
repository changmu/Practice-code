# coding=utf-8
import os
import scrapy


def join_strs_with_strip(ori_strs, sep=""):
    list = []
    for item in ori_strs:
        list.append(item.strip())
    return sep.join(list)


def extract_text_with_css(item, query):
    return item.css(query).get(default="").strip()


class DoubanBookSpider(scrapy.Spider):
    name = "douban_book"
    allowed_domains = ["book.douban.com"]
    start_urls = ["http://book.douban.com/top250"]
    rank = 1

    def parse(self, response):
        for book in response.css("tr.item"):
            title = join_strs_with_strip(
                book.css(".pl2 a ::text").getall()
            )  # 提取a标签下的所有文本

            author = extract_text_with_css(book, "p.pl::text")
            rating = extract_text_with_css(book, "span.rating_nums::text")
            comments = book.css("span.pl::text").re(r"[0-9]+")[0]
            inq = extract_text_with_css(book, "span.inq::text")

            # 组装简要信息
            parsed_book = {
                "排名": self.rank,  # 捕获值而非对象
                "书名": title,
                "作者": author,
                "评分": rating,
                "评价人数": int(comments),
                "一句话简介": inq,
            }
            self.rank += 1

            # 抓取详细信息
            yield scrapy.Request(
                url=book.css(".pl2 a::attr(href)").get(),
                callback=self.parse_book_detail,
                cb_kwargs={"parsed_book": parsed_book},
            )

        # 获取下一页
        next_page = response.css("span.next a::attr(href)").get()
        if next_page is not None:
            yield response.follow(next_page, callback=self.parse)

    def parse_book_detail(self, response, parsed_book):
        brief = response.css("span.hidden div.intro ::text")
        if len(brief) == 0:
            brief = response.css("#link-report div.intro ::text")
        parsed_book["内容简介"] = join_strs_with_strip(brief.getall())
        parsed_book["作者简介"] = join_strs_with_strip(
            response.css("div.related_info > div:nth-child(6) p ::text").getall()
        )
        # FIXME 摘录可以取完整版的
        parsed_book["原文摘录"] = join_strs_with_strip(
            response.css("ul.blockquote-list li:nth-child(1) ::text").getall()
        )
        # FIXME  热评可以优化成数组
        parsed_book["热评"] = join_strs_with_strip(
            response.css("p.comment-content ::text").getall(),
        )

        yield parsed_book
