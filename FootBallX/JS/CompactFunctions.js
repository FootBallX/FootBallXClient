// 全局函数:
// Log("abc");      输出log
// Rand();          取随机数，范围在0 ~ 最大正整数，如果要取0~100的随机数，则写：Rand() % 100;
// PlayAnimation(ccbi_name, delay);     // 播放动画，参数1是动画的ccbi文件名，delay是以秒为单位，表示动画播放的延迟开始时间。

var ballSpeed = 0;  // 球减速修正,需要在适当的时候初始化
var type = 0;       // 0 传球，1 盘带


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


function StartDribble(o1)
{
    type = 1;
}

function StartPassBall(o1, isAir)
{
    PlayAnimation("ground_daiqiu.ccbi", 0);
    ballSpeed = 100;
    type = 0;
    return;
}



// 铲球
function TackleBall(o1, o2)
{
    if (Rand() % 1000 > 500)
    {
        PlayAnimation("ground_chanqiu_success.ccbi", 0);
        return true;
    }
    else
    {
        PlayAnimation("ground_chanqiu_failed.ccbi", 0);
        return false;
    }
}


// 拦截
function InterceptBall(o1, o2)
{
	return true;
}


// 封堵
function BlockBall(o1, o2)
{
	return true;
}



function ReceiveBall(o1)
{
    PlayAnimation("ground_tingqiu.ccbi", 0);
	return;
}
