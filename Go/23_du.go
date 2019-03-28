package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
)

type dirInfo struct {
	dir_name   string
	item_count int
	dir_size   int64
}

func main() {
	flag.Parse()
	root_dirs := flag.Args()
	if len(root_dirs) == 0 {
		root_dirs = []string{"."}
	}

	dir_info_chan := make(chan dirInfo)

	// 遍历目录
	go func() {
		for _, dir := range root_dirs {
			walkDir(dir, dir_info_chan)
		}
		close(dir_info_chan)
	}()

	total_size := int64(0)
	for dir_info := range dir_info_chan {
		// fmt.Printf("dir_name:%s\titem_count:%d\tdir_size:%d\n", dir_info.dir_name, dir_info.item_count, dir_info.dir_size)
		total_size += dir_info.dir_size
	}
	fmt.Printf("total_size:%.2fGB\n", float64(total_size)/(1<<30))
}

func walkDir(dir string, dir_info_chan chan<- dirInfo) {
	listDir := func(dir string) []os.FileInfo {
		items, err := ioutil.ReadDir(dir)
		if err != nil {
			fmt.Printf("du:%s", err)
			return nil
		}
		return items
	}

	dir_info := dirInfo{
		dir_name:   dir,
		item_count: 0,
		// dir_size: 0
	}
	for _, item := range listDir(dir) {
		if item.IsDir() {
			subdir := filepath.Join(dir, item.Name())
			walkDir(subdir, dir_info_chan)
		} else {
			dir_info.item_count++
			dir_info.dir_size += item.Size()
		}
	}

	dir_info_chan <- dir_info
}
