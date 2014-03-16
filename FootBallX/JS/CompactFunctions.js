// 全局函数:
// Log("abc");      输出log
// Rand();          取随机数，范围在0 ~ 最大正整数，如果要取0~100的随机数，则写：Rand() % 100;
// PlayAnimation(ccbi_name, delay);     // 播放动画，参数1是动画的ccbi文件名，delay是以秒为单位，表示动画播放的延迟开始时间。

var ballSpeed = 0;  // 球减速修正,需要在适当的时候初始化



function GetSpeed(o1)
{
    var spd = o1.speed / 1000;
    return parseFloat(spd);
}

// o1,o2 分别表示两个球员的属性。
// 可以用o1.speed来引用球员的速度。
// 属性包括:
// id
// type
// strength
// speed
// dribble
// pass
// shoot
// defence
// take
// hit
// attack
// ground
// air
function ComputePassBallSuccessRate(o1, o2)
{
    return Rand() + o1.attack - o2.defence;
}



function StartPassBall(o1)
{
    PlayAnimation("g_daiqiu.ccbi", 0);
    return;
}




function TackleBall(o1, o2)
{
    PlayAnimation("g_tackle.ccbi", 0);
	return true;
}



function InterceptBall(o1, o2)
{
	return true;
}



function BlockBall(o1, o2)
{
	return true;
}



function RecieveBall(o1)
{
    PlayAnimation("g_recive.ccbi", 0);
	return;
}
