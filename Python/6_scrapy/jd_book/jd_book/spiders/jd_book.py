import scrapy

class JDBookSpider(scrapy.Spider):
    name = 'jd_book'
    start_urls = ['https://book.jd.com/booktop/0-0-0.html?category=1713-0-0-0-10001-1']

    def parse(self, rsp):
        fileName = 'jd_book.html'
        with open(fileName, 'wb') as f:
            f.write(rsp.body)
        self.log(f'saved {fileName}')