1. 我在读取数据的同时分割任务，我使用1000 个数据为一组，然后加入到线程中计算。<br>
2. 欧式距离要求两个数据集的点一样多，所以我复制了一份数据并修改其中部分点，从而进行测试，结果正确。<br>
3. 我没有做数据压缩等处理，由于每读1000 个数据即开始计算，数据存在各自线程的内存中，我开启了16个线程，数量过多超过16000 个数据，会等待已有的16个线程计算完成并释放资源才加入新任务，应该不会崩溃。<br>
4. 代码方法写得有些混乱，还需调整。<br>
5. 线程间通信，计算结果相互访问，还没有完成。<br>
***
## 2024/2/29
1. 我重构了大部分代码，包括读取数据、线程池以及欧式距离计算，并去除了第三方库tinyxml2 库的使用，只用标准库。<br>
2. 可多线程同时读取轨迹文件数据保存至内存，读取完所有轨迹后开始执行距离计算，我使用欧式距离计算方法，也可以换成其他方法，只需要扩展一个类以及计算方法即可。<br>
3. 我的欧式距离计算方法是：假设有轨迹A、轨迹B、轨迹C、轨迹D，如果计算轨迹A [a1,a2,a3...] 与轨迹B [b1,b2,b3...] 的距离：<br>
先选择轨迹A的点a1，遍历轨迹B的所有点找到与点 a1 距离(即差的平方之和(x1-x2)(x1-x2)+(y1-y2)(y1-y2))最近的点bn, 算成bn与a1的距离。<br>
然后选择轨迹A的点a2，重复上面步骤，把所有A的点的距离结果相加，同时再以轨迹B为基准，也得出一个距离。两个距离的平均值作为A与B间距离。<br>
4. 我原本打算是边读取轨迹文件边计算距离，但在计算的时候将会需要还未读取的轨迹，以及其他问题不好解决，所以放弃了这个想法。<br>
5. 每一个Euclidean对象对应一个轨迹，它保存了该轨迹到其他轨迹间的距离。<br>
6. 计算时间很长，我只用了5个100kb左右的轨迹文件，就用了 140 秒。<br>
7. 输出示例如下，距离结果还没有归一化处理，还未转换成相似度。<br>
> 11296197.gpx 与 11296434.gpx 间距离 = 8.04211e+06<br>
> 11296197.gpx 与 11291991.gpx 间距离 = 9.54957e+06<br>
> 11296197.gpx 与 11296271.gpx 间距离 = 9.41946e+06<br>
> 11296106.gpx 与 11296434.gpx 间距离 = 1.06507e+07<br>
> 11296106.gpx 与 11291991.gpx 间距离 = 4.696e+06<br>
> 11296106.gpx 与 11296271.gpx 间距离 = 6.3996e+06<br>
> 11296434.gpx 与 11291991.gpx 间距离 = 5.00373e+06<br>
> 11296434.gpx 与 11296271.gpx 间距离 = 4.07974e+06<br>

.gpx 是轨迹文件名<br>
## 2024/3/2
1. 使用min-max归一化处理：x = (x - min)/(max - min) 我将两两轨迹间的欧式距离保存在二维数组 result_vv[][] 中，从所有距离遍历搜寻max 和min
2. 我测试时设置相似度阈值0.5
3. 输出结果如下
> . . . . . . . . . . 用时：113s
> 11296197.gpx 与 11296106.gpx 相似度：0.581631
> 11296197.gpx 与 11296434.gpx 相似度：0.993585
> 11296197.gpx 与 11296271.gpx 相似度：0.577703
> 11296197.gpx 与 11291991.gpx 相似度：1
> 11296106.gpx 与 11296271.gpx 相似度：0.99944
> 11296434.gpx 与 11291991.gpx 相似度：0.990135
相似度为1的是归一化处理中的min
