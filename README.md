1.我在读取数据的同时分割任务，我使用1000 个数据为一组，然后加入到线程中计算。
2.欧式距离要求两个数据集的点一样多，所以我复制了一份数据并修改其中部分点，从而进行测试，结果正确。
3.我没有做数据压缩等处理，由于每读1000 个数据即开始计算，数据存在各自线程的内存中，我开启了16个线程，数量过多超过16000 个数据，会等待已有的16个线程计算完成并释放资源才加入新任务，应该不会崩溃。
4.代码方法写得有些混乱，还需调整。
5.线程间通信，计算结果相互访问，还没有完成
