# coding: utf-8
# 将一句话打印在屏幕中央的方框里
# Author：长木
# compiler version：2.7.9
# 时间：2015.1.4

sentence = raw_input(unicode('请输入一句话：', 'utf-8').encode('gbk'))

screen_width = 80
text_width = len(sentence)
box_width = text_width + 6
left_margin = (screen_width - box_width) // 2

print
print ' ' * left_margin + '+' + '-' * box_width + '+'
print ' ' * left_margin + '|' + ' ' * box_width + '|'
print ' ' * left_margin + '|   ' + sentence + '   |'
print ' ' * left_margin + '|' + ' ' * box_width + '|'
print ' ' * left_margin + '+' + '-' * box_width + '+'
print

raw_input()