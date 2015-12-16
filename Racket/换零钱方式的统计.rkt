#lang racket

#|
| 将总数为a的现金换成n种硬币的不同方式的数目等于
| ·将现金数a换成除第一种硬币外的硬币的不同方式数目 + 
| ·将现金数a - d换成所有种类的硬币的不同方式数目，其中d是第一种硬币的币值
| 如果a == 0，应该算作是有1种换零钱的方式。
| 如果a < 0 || n == 0，应该算作有0种换零钱的方式。
|#

(define (count-change amount)
  (cc amount 5))

(define (cc amount kinds-of-coins)
  (cond ((= amount 0) 1)
		((or (< amount 0) (= kinds-of-coins 0)) 0)
		(else (+ (cc amount (- kinds-of-coins 1))
				 (cc (- amount
						(first-denomination kinds-of-coins))
					 kinds-of-coins)))))

(define (first-denomination kinds-of-coins)
  (cond ((= kinds-of-coins 1) 1)
		((= kinds-of-coins 2) 5)
		((= kinds-of-coins 3) 10)
		((= kinds-of-coins 4) 25)
		((= kinds-of-coins 5) 50)))
; test
(count-change 100)
