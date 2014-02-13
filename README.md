# uniq　  簡易テキストモディファイラ


ローカルで uniq sort reverse sort , splitしたかった自分用
ちょっと`perl`でとか`tcsh`でとか`sed awk`やりたいときに素のWindowsだと`(;´Д｀)`ってなるので

###uniq

    $ cat hoge | uniq 

っぽいことができる  

![uniq](https://raw.github.com/katakk/uniq/master/doc/uniq.png)

![uniq](https://raw.github.com/katakk/uniq/master/doc/uniqa.png)

####単語ごとにuniq


![uinqw](https://raw.github.com/katakk/uniq/master/doc/uinqwa.png)

###ずれてるの整形したりできる  

sprintf %-50s %-50s %-50s っぽいかんじ  

![format](https://raw.github.com/katakk/uniq/master/doc/format.png)

![format](https://raw.github.com/katakk/uniq/master/doc/formata.png)

####ソートできる　　


論理ソート(StrCmpLogicalW API使う奴)と物理ソート(ただのqsort)があります　　

![sort](https://raw.github.com/katakk/uniq/master/doc/sort.png)


####ログ監視とか

    $ tail -f log 風味

![sort](https://raw.github.com/katakk/uniq/master/doc/tailf.png)


#### grep とか

#### grep -v とか


#### split とか

#### sed -e 's/ /\n/g' とか

###検索 置換機能

すこし独自。普通の検索置換は秀丸でやればいいと思う。




