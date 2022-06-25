# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://docs.scrapy.org/en/latest/topics/item-pipeline.html


# useful for handling different item types with a single interface
from itemadapter import ItemAdapter
import openpyxl


class ExcelPipeline:
    def __init__(self):
        self.wb = openpyxl.Workbook()
        self.ws = self.wb.active
        self.ws.title = "豆瓣图书榜"
        self.ws.append(("排名", "书名", "评分", "作者"))

    def close_spider(self, spider):
        self.wb.save("douban_book.xlsx")

    def process_item(self, item, spider):
        self.ws.append((item["排名"], item["书名"], item["评分"], item["作者"]))
        return item
