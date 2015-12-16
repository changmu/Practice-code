#lang racket

(define (factorial n)
  (define (factorial-iter result count)
	(if (> count n)
	  result
	  (factorial-iter (* result count)
					  (+ count 1))))
  
  (factorial-iter 1 1)
  )

"test"
(factorial 3)
(factorial 10)
(factorial 10000)
