import json
import time

from util import create_chrome_driver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions

if __name__ == "__main__":
    b = create_chrome_driver()
    b.get("https://login.taobao.com")
    # 等待时间 秒
    b.implicitly_wait(10)
    # 获取页面元素模拟用户输入和点击行为
    name_input = b.find_element(By.CSS_SELECTOR, "#fm-login-id")
    pass_input = b.find_element(By.CSS_SELECTOR, "#fm-login-password")
    name_input.send_keys("a")  # 用户名
    pass_input.send_keys("aa")  # 密码，后面还需要手机验证
    login_button = b.find_element(By.CSS_SELECTOR, "#login-form > div.fm-btn > button")
    login_button.click()
    # 显示等待
    # wait_obj = WebDriverWait(b, 10)
    # wait_obj.until(
    #     expected_conditions.presence_of_element_located(
    #         (By.CSS_SELECTOR, "#J_SiteNavLogin")
    #     )
    # )
    time.sleep(30)

    # 获取cookie数据写入文件
    with open("cook.json", "w") as f:
        json.dump(b.get_cookies(), f)
