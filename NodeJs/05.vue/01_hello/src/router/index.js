import { createRouter, createWebHashHistory } from 'vue-router'

export default createRouter({
  history: createWebHashHistory(),
  routes: [
    {
      path: "/DemoCounter",
      name: "DemoCounter",
      component: () => import('../components/C01DemoCounter.vue')
    },
    {
      path: "/MyAbout",
      name: "about",
      component: () => import('../components/MyAbout.vue')
    },
    {
      path: "/MarkdownEditor",
      name: "Markdown",
      component: () => import('../components/C02MarkdownEditor.vue')
    },
    {
      path: "/C03GithubCommits",
      name: "GitCommits",
      component: () => import('../components/C03GithubCommits.vue')
    },
    {
      path: "/C04AntD",
      name: "AntD",
      component: () => import('../components/C04AntD.vue')
    },
  ]
})