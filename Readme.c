/**********************************************************************
20191102
1，更改太阳能充电占空比低保护
2，新增充电口温度保护
3，增加USB过流报警 ，不关断
4，soc低于95后允许充电
20191104
1，充电器充电禁止AC放电
2，对充电低电压告警与电压从正常慢慢往下区别，防止充电器前端拔掉误报警
20191105
1，取消待机一小时关机
2，取消usb故障后，无usb放电，流水灯停转
20191107
1，新增点烟器输出电流高，开逆变器风扇
2，新增充电口温度高，开逆变器风扇
20191108
1，更改点烟器保护电流值
2，更改风扇开启温度
20191111
1，取消点烟器故障判断开启时间
2，更改点烟器保护电流值
---
1，更改12v保护电流值
2，更改充电电流采样阻值
20191113
1，更改充电器充电电压
2，更改充电器过流保护，过流后直接关闭
3，新增AC报警led闪
4，更改各图标显示阀值
20191115
1，更改充电器部分充电程序，PWM100%
2，开机初始化点烟器电流。后续校准使用
20191126
1，取消usb延时8s检测故障
20191127
1，修复ac低电流充电，指示灯可短暂开启
20191128
1，修复低电量报警时插入充电，反复触发报警
20191129
1，报警优化，统一为蜂鸣器响30s后，闪烁显示
2，低电量报警与故障报警同时存在时，响应故障报警。
20191130
1，增加测试模式，测试模式下不判断通信，使用电压判断电量
20191210
1，增加测试模式(充电)
2，优化PWM部分代码。
20191212
1，取消上版本PWM更改
2，更改点烟器电流基准132.33
3，更改点烟器电流校准曲线
20191213
1，DYQInit电压改动
2，pwmdeinit
20191214
1，修改AC输出过功率保护后，无清除计时bug
2，AC充电增加测试模式，放宽电压范围10.5-14.1
3，增加12V无插入状态下，电流限制。
4，修改低电量报警后插入低电压充电后，
	 低电量关机后无法亮屏bug。
5，更改为逆变300w。
20191224
1，更改AC充电电流限制采用占空比控制10A>5.7A
20191229
1，修复通信错误时SOC置0后无法恢复自动关机bug
2，增加满电后对采集板电量校准
3，增加逆变板后级故障无输出时AC报警
4，增加点烟器初始化时间限制与数据范围限制
20191231
1，修复开机初始化电量与运行时电量差异bug
2，优化开机点烟器电流初始化
3，更改AC充电电压范围
4，微调充电电流调整速度
5，微调12V过流保护电流
20200104
1，修复SUN充电告警时插入AC充电，SUN拔出后告警无法停止bug
2，中断用到数据定义为volatile
20200107/8/9
1，取消volatile类型定义
2，优化部分数据类型
20200210
1，增加宏定义快速切换AC充电10A，不通过PWM
20200521
1，删除无用else
2，发布版本A0
***********************************************************************/
