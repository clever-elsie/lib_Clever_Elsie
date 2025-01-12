## 高速フーリエ変換
高速フーリエ変換は、時間と周波数について離散値を扱うフーリエ変換の効率的なアルゴリズムだ。
フーリエ変換を時間について離散化したものが離散時間フーリエ変換で、それをさらに周波数についても離散化したものが離散フーリエ変換である。

高速フーリエ変換はその離散フーリエ変換についてデータ数$n$の畳み込み演算を$O(n\log n)$で行うことができる。
対数の底は定数倍に吸収されるのでどの値を入れてもいいですが、アルゴリズムの性質上$\log_2$と考えると都合がよい。

高速フーリエ変換を用いない普通の離散フーリエ変換では計算量は$O(n^2)$程度になる。
1億のデータに対しては少なくとも1日は終わらないくらい、非常に使い勝手が悪いものになる。
その点、高速フーリエ変換を用いれば1億のデータに対しても、実装言語によって多少の差はあっても数分以内で計算が終了する。

参考までに以下に各要素数に対して畳み込みを行った結果を記載する。

実行環境はUbuntu22.04, Intel Xeon E5 2650v4 $imes$ 2, DDR4-2400 128GBの自宅にある実験機である。
スループット性能重視で計算性能はかなり世代遅れで、最新の14世代i7の1/4程度のシングルスレッド性能なので本当に参考程度にしかならない。

|配列aのサイズ|配列bのサイズ|FFTの時間|DFTの時間|普通の2重forの時間|
|---|---|---|---|---|
|$10$|$10$|0.000005秒|0.000110秒|0.000003秒|
|$10^2$|$10^2$|0.000023秒|0.005394秒|0.000025秒|
|$10^3$|$10^3$|0.000169秒|0.383秒|0.00178秒|
|$10^4$|$10^4$|0.00327秒|35.2秒|0.146秒|
|$10^5$|$10^5$|0.0240秒|3485秒|12.3秒|
|$10^6$|$10^6$|0.193秒|未計測|1233秒|
|$10^7$|$10^7$|3.32秒|未計測|未計測|
|$10^8$|$10^8$|30.1秒|未計測|未計測|

FFT:[ac-library convolution](https://github.com/atcoder/ac-library/blob/master/document_ja/convolution.md)

DFT:[東京工業大学デジタル創作同好会traP DFTとIDFT](https://trap.jp/post/1386)

FFT,DFTを用いない畳み込みでは配列a,bを1つずつずらしながら積の和を配列に保存する2重forで畳み込みを計算。

表のとおり、FFTでは要素数にほぼ線形に時間が増加する。

計算量が$O(n^2)$のプログラムと$O(n\log n)$のプログラムでは計算量によって現実的に計算不可能になる領域が大きく差が付く。
これはクイックソートとバブルソートなどの関係とほぼ同じものと考えてよい。

以上からFFTを用いるモチベーションは明らかに計算時間的なメリットから大きなものになる。

FFTについて見る前に、離散フーリエ変換の仕組みを説明する。
離散フーリエ変換の式は以下で与えられる.ただし、$N\in\mathbb{N}$を周期、つまりデータ点の数とする。
$$
\begin{align*}
\mathrm{DFT}(f(t))&=F(k)=\sum_{n=0}^{N-1}f(n)e^{-i{2\pi k\over N}n}\quad (k=0,1,\cdots,N-1)\\
\mathrm{DFT}^{-1}(F(k))&=f(n)={1\over N}\sum_{k=0}^{N-1}F(k)e^{i{2\pi k\over N}n}
\end{align*}
$$

通常のフーリエ変換は以下の式で与えられるので比較する。
$$\begin{align}
\mathscr{F}(f(t))&=F(\omega)=\int_\mathbb{R} f(t)e^{-i\omega t}\mathrm{d}t\\
\mathscr{F}^{-1}(F(\omega))&=f(t)={1\over 2\pi}\int_\mathbb{R} F(\omega)e^{i\omega t}\mathrm{d}\omega
\end{align}$$

フーリエ変換とDFTで異なる点は主に2つで、前述の通り時間と周期について離散的か連続的かという点である。
もう一つ注目する点があるとすれば、フーリエ変換は積分区間が実数域全体にわたっているのに対して、DFTでは周期の範囲の総和をとっている。
これはフーリエ変換を離散時間フーリエ変換に拡張し、さらに離散フーリエ変換に拡張した過程を見れば明らかになる。

## 離散時間フーリエ変換
音声のようなデータを扱いたいとき、それらは44.1kHzや48kHzのような時間について不連続な関数とみなすことができる。
離散的な関数に対してフーリエ変換を施すときに問題になることが積分が0になってしまうことだ。
イメージとしては以下の無理やり線のように見えるように作った図の通りである。
有限のデータは連続な定義域で積分しても$n<\aleph$でつぶれてしまう。
周期$N$の無限のデータに対しても$\aleph_0<\aleph$なので、潰れることは自明である。
ただし$\aleph$は実数の濃度、$\aleph_0$は整数のような加算無限集合の濃度、$n\in\mathbb{N}$とする。

そこで、区分求積法の時のように不連続なデータに対して、$\displaystyle{2\pi\over N}$のデータの間隔分の幅を与えることで面積を与える。

データを上の図のように解釈すると、行う演算は積分から総和になるため、演算の難易度が一つ下がる。
式は以下のようになる。
$$\begin{align*}
F(\omega)=\sum_{n=-\infty}^\infty f(n)e^{-i\omega n}
\end{align*}$$
$f(n)$は入力となるデータの$n$番目の値なので、必ずしも関数である必要はなく数列の総和に$e^{-i\omega n}$をかけたものと解釈することも可能である。
また、面積を与えるためにディラックのデルタ関数を用いることもできる。
しかしいずれにせよ、この段階では$f(n)$は周期$N$の無限の数列なので、まだ計算可能ではない。

DTFTに対して、逆変換は以下のようになる。
$$\begin{align*}
f(n)={1\over2\pi}\int_{-\pi}^{\pi}F(\omega)e^{i\omega n}\mathrm{d}\omega
\end{align*}$$
以上の式は前提となる絶対可積分性から、DTFTの総和とIDTFTの積分を交換することができ、$\delta(n,m)$をクロネッカーのデルタとすると、以下のように逆変換が正しいことが確認できる。
$$
\begin{align*}
f(n)&={1\over 2\pi}\int_{-\pi}^{\pi}\left(\sum_{m=-\infty}^\infty f(m)e^{-i\omega m}\right)e^{i\omega n}\mathrm{d}\omega\\
&={1\over 2\pi}\sum_{m=-\infty}^\infty f(m)\int_{-\pi}^{\pi}e^{i\omega(n-m)}\mathrm{d}\omega\\
&={1\over 2\pi}\sum_{m=-\infty}^\infty f(m)2\pi\delta(n,m)\\
&={1\over 2\pi}2\pi f(n)\\
&=f(n)\\\\
\because\int_{-\pi}^{\pi}e^{i\omega(n-m)}\mathrm{d}\omega
&=
\begin{pmatrix}
	{1\over i(n-m)}\left[e^{i\omega(n-m)}\right]_{-\pi}^\pi & n\ne m\\
	\int_{-\pi}^\pi \mathrm{d}\omega & n=m
\end{pmatrix}\\
&=
\begin{pmatrix}
{1\over i(n-m)}\left((-1)^{n-m}-(-1)^{n-m}\right)  & n\ne m\\
\left[\omega\right]_{-\pi}^\pi & n=m
\end{pmatrix}\\
&=
\begin{pmatrix}
0 & n\ne m\\
2\pi & n=m\\
\end{pmatrix}\\
&=2\pi\delta(n,m)
\end{align*}
$$

## 離散フーリエ変換
離散時間フーリエ変換では離散時間のデータに対するフーリエ変換を行えるようになったが、まだ総和の計算が全整数に渡っていたり、逆変換に積分が必要であるという点で、計算機で扱いづらいものであることを確認した。

離散フーリエ変換(DFT)では周波数の離散化を行うことでようやく計算機で計算可能な状態まで変形させる。

DTFTによって非周期的な関数では連続なスペクトルが得られる。
逆に$f(n)$に周期性を仮定するとスペクトルは離散的になる。
これはフーリエ級数展開で周期関数を展開することで離散的なスペクトルを得られることと、フーリエ変換で非周期的な関数の変換が連続的なスペクトルになることの相似形であると考えてよい。

ここで、クロネッカーのデルタ関数の拡張としてのディラックのデルタ関数を導入する。
これはインパルス関数としても知られる$\displaystyle\int_{-\infty}^\infty f(x)\delta(x)\mathrm{d}x=f(0)$を満たす関数である。
これを$\delta(x-C),\;x,C\in\mathbb{R}$のようにすることで、定数$C$によってインパルスの位置を左右に移動させることができる。＠＠

離散的なスペクトルをとることを念頭に置き、$F(\omega)=\displaystyle\sum_{k=-\infty}^\infty c_k\delta\left(\omega-{2\pi k\over N}\right)$と置き換える。

周期性を仮定するため、$\forall i\in\mathbb{N},\; c_i = c_{i+n}$が成り立つ。

これを逆変換(IDFT)の式に代入する。
$$\begin{align*}
f(n)
&={1\over2\pi}
\int_{-\pi}^\pi F(\omega) e^{i\omega n} \mathrm{d}\omega\\
&={1\over2\pi}
	\int_{-\pi}^{\pi}
		\sum_{k=-\infty}^\infty
			c_k\delta\left(\omega-{2\pi k\over N}\right)
			e^{i\omega n}
	\mathrm{d}\omega\\
&={1\over2\pi}
	\sum_{k=0}^{N-1} c_k
		\int_{-\pi}^\pi
			\delta\left(\omega-{2\pi k\over N}\right)
			e^{i\omega n}
		\mathrm{d}\omega\\
&={1\over2\pi}
\sum_{k=0}^{N-1}
c_k e^{i{2\pi k\over N}n}
\end{align*}$$

$\displaystyle e^{i{2\pi k\over N}n}$はデルタ関数の性質から明らかである。
総和の区間が整数域全域から0から$N-1$の$N$個のデータに変わったのは周期性により$[-\pi,\pi]$の$2\pi$に含まれるデータは必ずちょうど$N$個の過不足無いデータであるからである。

以上の式から$c_k$を取り除くことで離散フーリエ逆変換が得らる。
ただし、$c_k=\displaystyle{2\pi\over N}F(k)$となる数列$F(k)$を導入する。
$$\begin{align*}
f(n)
&={1\over 2\pi}\sum_{k=0}^{N-1}{2\pi\over N}F(k)e^{i{2\pi k\over N}n}\\
&={1\over N}\sum_{k=0}^{N-1}F(k)e^{i{2\pi k\over N}n}
\end{align*}$$
以上が離散フーリエ逆変換である。


さらに離散フーリエ変換を求める。周期性を仮定したためDTFTの式では絶対化積分性が失われる。したがって和をとる区間を1周期分に限り、以下のようになる。
$$\begin{align*}
F(k)=\sum_{n=0}^{N-1}f(n)e^{-i{2\pi k\over N}n}
\end{align*}$$


これが逆変換によって元の式に戻ることを確認する。
$$\begin{align*}
{1\over N}\sum_{k=0}^{N-1}\sum_{m=0}^{N-1}f(m)e^{-i{2\pi k\over N}m}e^{i{2\pi k\over N}n}
&={1\over N}\sum_{m=0}^{N-1}f(m)\sum_{k=0}^{N-1}e^{i{2\pi k\over N}(n-m)}\\
&={1\over N}\sum_{m=0}^{N-1}f(m)N\delta_{m,n}\\
&=f(n)
\end{align*}$$
クロネッカーのデルタが現れる理由は$\int e^{ax}={1\over a}e^{ax}$よりIDTFTの積分の時と同じである。


参考文献

http://www.ic.is.tohoku.ac.jp/~swk/lecture/yaruodsp/dtft.html

http://www.ic.is.tohoku.ac.jp/~swk/lecture/yaruodsp/dft.html

https://ja.wikipedia.org/wiki/%E3%83%87%E3%82%A3%E3%83%A9%E3%83%83%E3%82%AF%E3%81%AE%E3%83%87%E3%83%AB%E3%82%BF%E9%96%A2%E6%95%B0

https://qiita.com/uyuutosa/items/9124d94aa56ccc136072


## FFT
さて、本題に戻ろう。

時間と周波数について離散化したDFTはある$n$について$[0,N)$の総和をとるため、$N$個のデータに対して$O(N^2)$の計算時間が必要になることは明らかである。これを高速化するためにFFTでは分割統治法による問題の簡単化を行う。そのため$N$を2のべき乗の形で表せるものに限定し、$2^c<N<2^{c+1}$ の形のものは$N=2^{c+1}$にして、残りを線形補完や0埋めで補う。


まずはDFTを行列で表す。あるベクトル$\boldsymbol{a}$をDFTして$\boldsymbol{b}$とする行列$W$を以下で表す。
$$
\begin{align*}
\boldsymbol{b}&=W\boldsymbol{a}\\
\begin{bmatrix}
	b_0\\ b_1\\ \vdots\\ b_{N-1}
\end{bmatrix}
&=\begin{bmatrix}
	w_N^0 &w_N^0 &w_N^0 &\cdots &w_N^0\\
	w_N^0 &w_N^1 &w_N^2 &\cdots &w_N^{N-1}\\
	w_N^0 &w_N^2 &w_N^4 &\cdots &w_N^{2(N-1)}\\
	\vdots&\vdots&\vdots&\ddots&\vdots\\
	w_N^0 &w_N^{N-1} &w_N^{2(N-1)} &\cdots &w_N^{(N-1)^2}
\end{bmatrix}
\begin{bmatrix}
	a_0\\ a_1\\ \vdots\\ a_{N-1}
\end{bmatrix}
\end{align*}$$
ただし$W$の$I$行$J$列目の要素は $\displaystyle w_N^{IJ}=e^{i{2\pi\over N}IJ}$である。
これはDFTの式$F(k)=\displaystyle \sum_{k=0}^{N-1} f(n)\exp\left(i{2\pi\over N}kn\right)$から明らかである。


ところで、この$w_N^{IJ}$は$N$乗ごとに同じ値をとる周期関数である。
ここで$N$が2のべき乗であることを考慮しながら$\boldsymbol{b}=W\boldsymbol{a}$の行が偶数の部分のみを取り出す。
ただし、簡単のため右下の$N$は省略する。
$$\begin{align*}
\begin{bmatrix}
b_0\\b_2\\b_4\\\vdots\\b_{N-2}
\end{bmatrix}
=\begin{bmatrix}
	w^0 &w^0 &\cdots &w^0 &w^0 &w^0 &\cdots &w^0\\
	w^0 &w^2 &\cdots &w^{N-2} &w^N &w^{N+2} &\cdots &w^{2N-2}\\
	w^0 &w^4 &\cdots &w^{2N-4} &w^{2N} &w^{2N+4} &\cdots &w^{4N-4}\\
	\vdots&\vdots&\ddots&\vdots&\vdots&\vdots&\ddots&\vdots\\
	w^0 &w^{N-2}&\cdots &w^{(N/2-1)(N-2)} &w^{(N-2)N/2} &w^{(n/2+1)(n-1)}&\cdots &w^{(n-1)(n-2)}
\end{bmatrix}
\begin{bmatrix}
	a_0\\
	a_1\\
	a_2\\
	\vdots\\
	a_{N-1}
\end{bmatrix}
\end{align*}$$


上の係数行列では、右半分と左半分で係数がその周期性により一致する。
これは行が偶数番目であるため
$2I$として
$J$列目の要素と
$J+N/2$列目の要素について
$w_N^{2I(J+N/2)}=w_N^{2IJ+NI}=w_N^{2IJ}$
となるためである。


したがって、行列の右半分については$\boldsymbol{a}$の後半を前半に足すことで省略できる。具体的には以下のようになる。
$$\begin{align*}
\begin{bmatrix}
b_0\\b_2\\b_4\\\vdots\\b_{N-2}
\end{bmatrix}
&=\begin{bmatrix}
	w^0_N&w^0_N&\cdots&w^0_N\\
	w^0_N&w^2_N&\cdots&w^{N-2}_N\\
	w^0_N&w^4_N&\cdots&w^{2N-4}_N\\
	\vdots&\vdots&\ddots&\vdots\\
	w^0_N&w^{N-2}_N&\cdots&w^{(N/2-1)(N-2)}_N
\end{bmatrix}
\begin{bmatrix}
	a_0+a_{N/2}\\
	a_1+a_{N/2+1}\\
	a_2+a_{N/2+2}\\
	\vdots\\
	a_{N/2-1}+a_{N-1}
\end{bmatrix}\\
&=\begin{bmatrix}
	w^0_{N/2}&w^0_{N/2}&\cdots&w^0_{N/2}\\
	w^0_{N/2}&w^1_{N/2}&\cdots&w^{N/2-1}_{N/2}\\
	w^0_{N/2}&w^2_{N/2}&\cdots&w^{N-2}_{N/2}\\
	\vdots&\vdots&\ddots&\vdots\\
	w^0_{N/2}&w^{N/2-1}_{N/2}&\cdots&w^{(N/2-1)^2}_{N/2}
\end{bmatrix}
\begin{bmatrix}
	a_0+a_{N/2}\\
	a_1+a_{N/2+1}\\
	a_2+a_{N/2+2}\\
	\vdots\\
	a_{N/2-1}+a_{N-1}
\end{bmatrix}
\end{align*}$$


これは、もともと求めたかった$W_N$に対して半分のサイズになっている。つまり$W_{N/2}$となっている。
計算量を$O(N\log_2 N)$まで落とせることを示すためには、奇数の行でも同じ性質が成り立つことを示し、再帰的に処理できることを確かめなければならない。
ではそのために奇数番目の行でも似た性質が成り立つことを確認する。


$$\begin{align*}
\begin{bmatrix} b_1\\b_3\\b_5\\\vdots\\b_{N-1}\end{bmatrix}
=\begin{bmatrix}
w^0&w^1 &\cdots&w^{N/2-1} &w^{N/2} &w^{N/2+1} &\cdots&w^{N-1}\\
w^0&w^3 &\cdots&w^{3N/2-3}&w^{3N/2}&w^{3N/2+3}&\cdots&w^{3(N-1)}\\
w^0&w^5 &\cdots&w^{5N/2-5}&w^{5N/2}&w^{5N/2+5}&\cdots&w^{5(N-1)}\\
\vdots&\vdots&\ddots&\vdots&\vdots&\vdots&\ddots&\vdots\\
w^0&w^{N-1}&\cdots&w^{(N/2-1)(N-1)}&w^{(N-1)N/2}&w^{(N/2+1)(N-1)} &\cdots&w^{(N-1)^2}
\end{bmatrix}
\begin{bmatrix} a_0\\a_1\\a_2\\\vdots\\ a_{N-1}\end{bmatrix}
\end{align*}$$


ここで、$w_N=\exp\left(\displaystyle -i{2\pi\over N}\right)$より、
$w_N^{N/2}=-w_N^N$となることに注意すれば、
$W$の左右をまとめて$\boldsymbol{a}$の上半分から下半分を引き以下のようになる。


$$\begin{align*}
\begin{bmatrix} b_1\\b_3\\b_5\\\vdots\\ b_{N-1}\end{bmatrix}
&=\begin{bmatrix}
	w^0_N&w^1_N &\cdots&w^{N/2-1}_N\\
	w^0_N&w^3_N &\cdots&w^{3N/2-3}_N\\
	w^0_N&w^5_N &\cdots&w^{5N/2-5}_N\\
	\vdots&\vdots&\ddots&\vdots\\
	w^0_N&w^{N-1}_N&\cdots&w^{(N/2-1)(N-1)}_N
\end{bmatrix}
\begin{bmatrix}
	a_0-a_{N/2}\\
	a_1-a_{N/2+1}\\
	a_2-a_{N/2+2}\\
	\vdots\\
	a_{N/2-1}-a_{N-1}
\end{bmatrix}\\
&=\begin{bmatrix}
	w^0_N&w^0_N &\cdots&w^0_N\\
	w^0_N&w^2_N &\cdots&w^{2(N/2-1)}_N\\
	w^0_N&w^4_N &\cdots&w^{4(N/2-1)}_N\\
	\vdots&\vdots&\ddots&\vdots\\
	w^0_N&w^{N-2}_N&\cdots&w^{(N/2-1)(N-2)}_N
\end{bmatrix}
\begin{bmatrix}
	w^0(a_0-a_{N/2})\\
	w^1(a_1-a_{N/2+1})\\
	w^2(a_2-a_{N/2+2})\\
	\vdots\\
	w^{N/2-1}(a_{N/2-1}-a_{N-1})
\end{bmatrix}\\
&=\begin{bmatrix}
	w^0_{N/2}&w^0_{N/2} &\cdots&w^0\\
	w^0_{N/2}&w^1_{N/2} &\cdots&w^{N/2-1}_{N/2}\\
	w^0_{N/2}&w^2_{N/2} &\cdots&w^{2(N/2-1)}_{N/2}\\
	\vdots&\vdots&\ddots&\vdots\\
	w^0_{N/2}&w^{N/2-1}_N&\cdots&w^{(N/2-1)^2}_N
\end{bmatrix}
\begin{bmatrix}
	w^0(a_0-a_{N/2})\\
	w^1(a_1-a_{N/2+1})\\
	w^2(a_2-a_{N/2+2})\\
	\vdots\\
	w^{N/2-1}(a_{N/2-1}-a_{N-1})
\end{bmatrix}\\
\end{align*}$$


以上から、数列のインデックスの偶奇で分けて演算を行うことで、問題のサイズを半分にすることが確認できたため、$\log_2 N$回の再帰的な操作によってDFTを行えることが確認できた。
さらに、高速な逆変換も求める。


逆変換を行う場合、$\boldsymbol{a}=FW\boldsymbol{a}$が成り立つ$F=W^{-1}$が存在すればいい。

ここで、$W$の要素$w_N$は1の$n$乗根であり、
$W$が対称行列であることから、
$F$の$I$行$J$列要素は$w_N^{-IJ}$とすることで、
$FW=nI_N$となる。
ただし$I_N$は$Nimes N$の単位行列とする。
これは、$FW$の$I$行$J$列目の要素が
$\displaystyle\sum_{k=0}^{N-1} w^{-Ik}w^{kJ}=n\delta(I,J)$となることから明らかである。
ただし、$\delta(I,J)$はクロネッカーのデルタ関数とする。

したがって、求める$F=W^{-1}$は$W$の各要素の指数に-1を掛けた行列の${1\over n}$倍である。


このとき、明らかにFFTと同様の手順で分割統治法を適用可能である。


以上でFFTに関するレポートを終わる。

さらに踏み込んだ内容として、計算の定数倍削減のためのバタフライ演算や、整数演算を行うための数論変換などもあげられるが、どちらも計算機の都合からの最適化アプローチであるため微分積分や線形代数を基礎にした1年生向けの説明には向かないものと考えた。

参考文献

https://qiita.com/51_24_11_/items/11c48395603670ea98d8#3-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E3%81%AE%E3%81%9F%E3%82%81%E3%81%AEdftidft

https://trap.jp/post/1386/
]
}
