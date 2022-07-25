import scrapy


def join_strs_with_strip(strs, sep=""):
    list = []
    for item in strs:
        list.append(item.strip())
    # return sep.join(list)
    return list


class GegedownnSpider(scrapy.Spider):
    name = "gegedownn"
    allowed_domains = ["gegedownn.com"]
    start_urls = ["https://www.gegedownn.com/35/35367/57860406_2.html"]

    def parse(self, response):
        txt = join_strs_with_strip(response.css(".content-body ::text").getall())
        self.crawler.stats.inc_value("page_size")
        yield {
            "page_size": self.crawler.stats.get_value("page_size"),
            "txt": txt,
        }

        # 获取下一页
        next_page = response.css("li.next a::attr(href)").get()
        if next_page is not None:
            yield response.follow(next_page, callback=self.parse)
