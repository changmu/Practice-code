#lang racket

#|
| ������Ϊa���ֽ𻻳�n��Ӳ�ҵĲ�ͬ��ʽ����Ŀ����
| �����ֽ���a���ɳ���һ��Ӳ�����Ӳ�ҵĲ�ͬ��ʽ��Ŀ + 
| �����ֽ���a - d�������������Ӳ�ҵĲ�ͬ��ʽ��Ŀ������d�ǵ�һ��Ӳ�ҵı�ֵ
| ���a == 0��Ӧ����������1�ֻ���Ǯ�ķ�ʽ��
| ���a < 0 || n == 0��Ӧ��������0�ֻ���Ǯ�ķ�ʽ��
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
