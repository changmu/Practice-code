<template>
  <h1>测试antd的用法</h1>
    <a-button type="primary" @click="click">Primary Button</a-button>
    <a-button>Default Button</a-button>
    <a-button type="dashed">Dashed Button</a-button>
    <a-button type="text">Text Button</a-button>
    <a-button type="link">Link Button</a-button>
    <hr>
  <h1>测试ElementUI</h1>
  <el-button type="success">el按钮</el-button>
</template>

<script setup>

const click = () => {
  console.log('button was clicked, this=', this);
}

</script>
 
<style>
h1 {

}
</style>