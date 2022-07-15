import { createApp } from 'vue'
import App from './App.vue'
// import { createRouter, createWebHashHistory } from 'vue-router'
import router from './router/index.js'

createApp(App)
  .use(router)
  .mount('#app')