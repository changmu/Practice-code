import { createApp } from 'vue'
import App from './App.vue'
// import { createRouter, createWebHashHistory } from 'vue-router'
import router from './router/index.js'

import { Button, DatePicker, Space } from 'ant-design-vue'
import 'ant-design-vue/dist/antd.css';

import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'

createApp(App)
  .use(router)
  .use(Button)
  .use(DatePicker)
  .use(Space)
  .use(ElementPlus)
  .mount('#app')