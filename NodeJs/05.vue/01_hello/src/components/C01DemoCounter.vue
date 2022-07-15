<template>
  <div id="counter">
    <h3>计数器：{{count}}</h3>
    <span>组件创建时间: {{ctime}}</span>
    <hr>
    <span>组件激活时间: {{activeTime}}</span><hr>
    <span>上次组件离开时间: {{deActiveTime}}</span>
    <h3>{{msg}}</h3>
    <button @click="handleClick">点我逆序字符串</button><br>
    <input type="text" v-model="msg">
    <h3>{{msg}}</h3>
    <h3 v-if="count%2">v-if当count为奇数时我才出现</h3>
    <h3 :disabled="count%2">:disable,当count为奇数时我才出现</h3>
    <h3 v-show="count%2 === 0">v-show当count为偶数时我才出现</h3>
    <br>
    <span v-html="rawHTML"></span>
    
  </div>
</template>

<script>
import { onActivated, onDeactivated, ref } from '@vue/runtime-core'

function now() {
  return new Date().toLocaleString()
}

export default {
  name: 'DemoCounter',
  setup() {
    const ctime = now()
    let activeTime = ref(now())
    let deActiveTime = ref(now())
    let count = ref(0)
    let timer = null;
    let msg = ref('hello, wind')
    let rawHTML = '<strong>测试v-html</strong>'

    onActivated(()=>{
      console.log('onActivated called');
      
      activeTime.value = now()
      timer = setInterval(() => {
        console.log('@count++');
        count.value++;
      }, 1000);
    })

    onDeactivated(()=>{
      console.log('onDeactivated called');
      
      deActiveTime.value = now()
      clearInterval(timer)
    })

    function handleClick() {
      msg.value = msg.value.split('').reverse().join('')
    }

    return {
      ctime,
      activeTime,
      deActiveTime,
      count,
      msg,
      rawHTML,

      handleClick,
    }
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>

</style>
