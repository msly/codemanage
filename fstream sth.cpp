ate:打开文件之后令读写位置移至文件尾端（at end）
trunc:将先前的文件内容移除（Remove)

ios::trunc存在的理由：
ios::trunc|ios::in|ios::out   在C中标志是"W+"
ios::in|ios::out              在C中标志是"r+"

