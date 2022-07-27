from unittest import result
import requests
import logging
import re
from urllib.parse import urljoin

logging.basicConfig(
    level=logging.INFO, format="%(asctime)s - %(levelname)s: %(message)s"
)

BASE_URL = "https://ssr1.scrape.center"
TOTAL_PAGE = 10


# 爬取网页实现
def scrape_page(url):
    logging.info("scraping %s...", url)
    try:
        response = requests.get(url)
        if requests.codes.ok == response.status_code:
            return response.text
        logging.error("scrape %s status_code %s", url, response.status_code)
    except requests.RequestException:
        logging.error("scraping err:%s", url, exc_info=True)


# 爬取列表页
def scrape_index(page):
    index_url = f"{BASE_URL}/page/{page}"
    return scrape_page(index_url)


# 解析列表页
def parse_index(html):
    pattern = re.compile('<a.*?href="(.*?)".*?class="name">')
    items = re.findall(pattern, html)
    if not items:
        return []
    for item in items:
        detail_url = urljoin(BASE_URL, item)
        logging.info("get detail url:%s", detail_url)
        yield detail_url


def main():
    for page in range(1, TOTAL_PAGE + 1):
        index_html = scrape_index(page)
        detail_urls = parse_index(index_html)
        logging.info("detail urls:%s", list(detail_urls))


if __name__ == "__main__":
    main()
