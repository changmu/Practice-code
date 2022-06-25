import scrapy


def get_text(item):
    return item.get(default="").strip()


def join_strs_with_strip(ori_strs, sep=""):
    list = []
    for item in ori_strs:
        list.append(item.strip())
    return sep.join(list)


class TaobaoGoodsSpider(scrapy.Spider):
    name = "taobao_goods"
    allowed_domains = ["taobao.com"]
    start_urls = ["https://taobao.com/"]

    def start_requests(self):
        keywords = ["手机", "笔记本", "鼠键套装"]
        keywords = ["手机"]
        for word in keywords:
            for page in range(2):
                url = f"https://s.taobao.com/search?q={word}&s={page*48}"
                yield scrapy.Request(url=url)

    def parse(self, response):
        for item in response.css("div.ctx-box"):
            yield {
                "title": join_strs_with_strip(item.css(".title a ::text").getall()),
                "price": get_text(item.css(".price strong::text")),
                "del_count": get_text(item.css(".deal-cnt::text")),
                "shop": get_text(item.css(".shop a span:last-child::text")),
                "location": get_text(item.css(".location::text")),
            }
