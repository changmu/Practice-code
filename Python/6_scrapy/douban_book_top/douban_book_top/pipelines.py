# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://docs.scrapy.org/en/latest/topics/item-pipeline.html


# useful for handling different item types with a single interface
from itemadapter import ItemAdapter
import openpyxl
import pymysql

# 写数据到mysql
class MysqlPipeline:
    def __init__(self):
        self.conn = pymysql.connect(
            user="root", password="123456", host="127.0.0.1", port=3306, database="test"
        )
        self.cursor = self.conn.cursor()

    def close_spider(self, spider):
        self.conn.close()

    def process_item(self, item, spider):
        self.cursor.execute(
            "insert into t_spider(ranking,title,rating,author) values(%s,%s,%s,%s)",
            (item["排名"], item["书名"], item["评分"], item["作者"]),
        )
        self.conn.commit()
        return item


# 写数据到excel
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
