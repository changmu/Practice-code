<template>
  <h2>vue3 demo</h2>
  <div id="left">
    <li v-for="route in routes" :key="route.path">
      <router-link :to="route.path">
        {{route.name}}
      </router-link>
    </li>
  </div>
  <div id="right">
    <router-view v-slot="{ Component }">
      <keep-alive>
        <component :is="Component" />
      </keep-alive>
    </router-view>
  </div>
</template>

<script setup>
import router from './router/index.js'
const routes = router.getRoutes()
</script>

<style scoped>
#left {
  width: 120px;
  height: 500px;
  float: left;
  border: 1px solid blue;
  padding: 5px;
  margin-right: 10px;
}

#right {
  width: 600px;
  height: 500px;
  float: left;
  /* border: 1px solid blue; */
  padding: 5px;
}

li {
  font-size: 4px;
}
</style>
