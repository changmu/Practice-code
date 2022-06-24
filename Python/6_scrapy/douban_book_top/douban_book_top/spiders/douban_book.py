# coding=utf-8
import scrapy


class DoubanBookSpider(scrapy.Spider):
    name = "douban_book"
    allowed_domains = ["book.douban.com"]
    start_urls = ["http://book.douban.com/top250"]

    def parse(self, response):
        def extract_text_with_css(item, query):
            return item.css(query).get(default="").strip()

        def parse_title(titles):
            title = ""
            for item in titles:
                title += item.strip()
            return title

        books = response.css("tr.item")
        for book in books:
            title = parse_title(book.css(".pl2 a ::text").getall())

            author = extract_text_with_css(book, "p.pl::text")
            rating = extract_text_with_css(book, "span.rating_nums::text")
            comments = book.css("span.pl::text").re(r"[0-9]+")[0]
            inq = extract_text_with_css(book, "span.inq::text")

            self.log(f"title:{title}")
            yield {
                "书名": title,
                "作者": author,
                "评分": rating,
                "评价人数": int(comments),
                "一句话简介": inq,
            }

        # 获取下一页
        next_page = response.css("span.next a::attr(href)").get()
        if next_page is not None:
            yield response.follow(next_page, callback=self.parse)
