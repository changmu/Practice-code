APP        := crawler
TARGET     := crawler
ENV        := test
INS        :=
app        := "crawler"

export CGO_CXXFLAGS_ALLOW:=.*
export CGO_LDFLAGS_ALLOW:=.*
export CGO_CFLAGS_ALLOW:=.*

.PHONY: all test clean

all: build
check:
ifeq ($(strip $(BK_CI_PIPELINE_NAME)),)
	@echo "\033[32m <============== 合法性校验 app ${app} =============> \033[0m"
	goimports -format-only -w -local git.code.oa.com .
	gofmt -s -w .
	golangci-lint run
endif

build:
	@echo "\033[32m <============== making app ${app} =============> \033[0m"
	go build -ldflags='-w -s' $(FLAGS) -o ./${app} ./

api-test: $(DEPENDENCIES)
	@echo -e "\033[32m ============== making api test =============> \033[0m"
	go test -v -coverpkg=./... ./api_test -c -o ./${app}_API.test

unit-test: $(DEPENDENCIES)
	@echo -e "\033[32m ============== making unit test =============> \033[0m"
	go test `go list ./... |grep -v api_test` -v -gcflags=all=-l -covermode=count -coverprofile=_cover.out  -run='^Test' ./... -tags static
	go tool cover -html=_cover.out -o cover.html
	@echo -e "\033[32m <============== 单测全部通过 =============> \033[0m"
	open cover.html

clean:
	@echo -e "\033[32m ============== cleaning files =============> \033[0m"
	rm -fv ${TARGET}

# 一键部署(默认test环境，可通过 make upload ENV=pre 来指定预发布)
upload:
	@echo "\033[32m <============== 设置交叉编译参数 =============> \033[0m"
	CGO_ENABLED=0 GOOS=linux GOARCH=amd64 make build
	@echo -e "\033[32m ============== uploading ${TARGET} =============> \033[0m"
	dtools bpatch -lang=go -env=${ENV} -app=${APP} -server=${TARGET} -user=${USER} -bin=${TARGET} -instances=${INS}
