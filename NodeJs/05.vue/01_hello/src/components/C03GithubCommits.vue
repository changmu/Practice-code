<template>
  <h1>NodeJs commits</h1>
  <template v-for="(branch, index) in branches" :key="index">
    <input type="radio" 
      :id="branch"
      :value="branch"
      name="branch"
      v-model="currentBranch">
    <label :for="branch">{{branch}}</label>
  </template>

  <p>nodejs/node@{{currentBranch}}</p>
  <ul>
    <li v-for="{html_url, sha, author, commit} in commits" :key="sha">
      <a :href="html_url" target="_blank" class="commit">{{sha.slice(0, 7)}}</a>
      - <span class="message">{{truncate(commit.message)}}</span><br>
      by <span class="author">
        <a :href="author.html_url" target="_blank">{{commit.author.name}}</a>
      </span>
      at <span class="date">{{formatDate(commit.author.date)}}</span>
    </li>
  </ul>
</template>

<script setup>
import { ref, watchEffect } from 'vue'

const API_URL = `https://api.github.com/repos/nodejs/node/commits?per_page=3&sha=`
const branches = ['main', 'dep', 'v10.x', 'v9.x', 'vnoexist']

const currentBranch = ref(branches[0])
const commits = ref(null)

watchEffect(async () => {
  const url = `${API_URL}${currentBranch.value}`
  commits.value = await (await fetch(url)).json()
})

function truncate(v) {
  const newLine = v.indexOf('\n')
  return newLine > 0 ? v.slice(0, newLine) : v
}

function formatDate(v) {
  return v.replace(/T|Z/g, ' ')
}
</script>

<style scoped>
a {
  text-decoration: none;
  color: #42b883;
}
li {
  line-height: 1.5em;
  margin-bottom: 20px;
}
.author,
.date {
  font-weight: bold;
}
</style>