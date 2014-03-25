
// 全局函数:
// Log("abc");                          输出log
// Rand();                              取随机数，范围在0 ~ 最大正整数，如果要取0~1000的随机数，则写：Rand() % 1000;
// PlayAnimation(ccbi_name, delay);     播放动画，参数1是动画的ccbi文件名，delay是以秒为单位，表示动画播放的延迟开始时间。
//
// o1,o2 分别表示两个球员的属性。可以用o1.speed来引用球员的速度。
// 
// 属性包括:
// id       ID
// g_type     司职
// strength 体力
// speed    速度
// dribble  盘带
// pass     传球
// shoot    射门
// defence  防守
// take     接球
// hit      击球
// attack   出击
// ground   地面
// air      空中

var g_ballSpeed = 0;              //球减速修正,需要在适当的时候初始化
var g_isAir = false;              
var g_type = 0;
//0 = 地面 传球
//1 = 地面 射门
//2 = 地面 盘带
//3 = 地面 二过一
//4 = 空中 传球
//5 = 空中 射门
//6 = 空中 盘带（停球）

//指令修正
var g_OrderParam_1 = 0;             //地面 传球vs铲球
var g_OrderParam_2 = 0;             //地面 传球vs拦截
var g_OrderParam_3 = 0;             //地面 传球vs封堵
var g_OrderParam_4 = 100;           //地面 盘带vs铲球
var g_OrderParam_5 = -100;          //地面 盘带vs拦截
var g_OrderParam_6 = 300;           //地面 盘带vs封堵
var g_OrderParam_7 = -300;          //地面 二过一vs铲球
var g_OrderParam_8 = 0;             //地面 二过一vs拦截
var g_OrderParam_9 = 0;             //地面 二过一vs封堵
var g_OrderParam_10 = -300;         //地面 射门vs铲球
var g_OrderParam_11 = -50;          //地面 射门vs拦截
var g_OrderParam_12 = 150;          //地面 射门vs封堵
var g_OrderParam_13 = 150;          //进攻空中 盘带vs解围
var g_OrderParam_14 = -100;         //进攻空中 盘带vs拦截
var g_OrderParam_15 = -300;         //进攻空中 盘带vs封堵
var g_OrderParam_16 = 150;          //进攻空中 盘带vs出击
var g_OrderParam_17 = 0;            //进攻空中 传球vs解围
var g_OrderParam_18 = 0;            //进攻空中 传球vs拦截
var g_OrderParam_19 = 0;            //进攻空中 传球vs封堵
var g_OrderParam_20 = 150;          //进攻空中 传球vs出击
var g_OrderParam_21 = -300;         //进攻空中 射门vs解围
var g_OrderParam_22 = -50;          //进攻空中 射门vs拦截
var g_OrderParam_23 = 150;          //进攻空中 射门vs封堵
var g_OrderParam_24 = 150;          //进攻空中 射门vs出击
var g_OrderParam_25 = 0;            //防守空中 传球vs拦截
var g_OrderParam_26 = 0;            //防守空中 传球vs封堵
var g_OrderParam_27 = -50;          //防守空中 解围vs拦截
var g_OrderParam_28 = 150;          //防守空中 解围vs封堵
var g_OrderParam_29 = -100;         //守门 接球
var g_OrderParam_30 = 0;            //守门 击球
var g_OrderParam_31 = 250;          //单刀 封堵盘带vs盘带
var g_OrderParam_32 = -250;         //单刀 封堵盘带vs射门
var g_OrderParam_33 = -250;         //单刀 封堵射门vs盘带
var g_OrderParam_34 = 250;          //单刀 封堵射门vs射门

//计算结果参数
var g_WinParam_1 = 600;		          //无减速，进攻方胜利
var g_RandomParam_1 = 400;	        //无减速，随机球
var g_WinParam_2 = 750;		          //有减速，进攻方胜利
var g_RandomParam_2 = 600;	        //有减速，随机球
var g_ReducedParam_2 = 250;         //有减速，减速
var g_ReducedParam = 100;           //减速修正


function GetSpeed(o1)
{
  var spd = o1.speed / 1000;
  return parseFloat(spd);
}

//传球
function StartPassBall(o1, isAir)
{
  if (g_isAir)
    {
      PlayAnimation("air_chuanqiu.ccbi", 0);
    g_ballSpeed = 0;
    g_type = 4;
    }
  else
    {
      PlayAnimation("ground_chuanqiu.ccbi", 0);
    g_ballSpeed = 0;
    g_type = 0;
    }
  return;
}

// 铲球
function TackleBall(o1, o2)
{
  switch (g_type)		
	{
    case 0 :  //铲球vs传球
    var v = Rand() % 1000 + ( o1.pass + o2.ground ) - ( o2.defence + o2.ground ) + g_OrderParam_1 + g_ballSpeed;
    if (v >= g_WinParam_2)
    {
      PlayAnimation("ground_chanqiu_failed.ccbi", 0);
      return false;
    }
      else if (v >= g_RandomParam_2)
    {
      PlayAnimation("ground_chanqiu_randomball.ccbi", 0);
      return false;
    }
    else if (v >= g_ReducedParam_2)
    {
      PlayAnimation("ground_chanqiu_reduced.ccbi", 0);
      g_ballSpeed += g_ReducedParam;
      return false;
    }
    else
    {
      PlayAnimation("ground_chanqiu_success.ccbi", 0);
      return true;
    }
   
    case 1 : //铲球vs射门
    var v = Rand() % 1000 + ( o1.shoot + o2.ground ) - ( o2.defence + o2.ground ) + g_OrderParam_10 + g_ballSpeed;
    if (v >= g_WinParam_2)
    {
      PlayAnimation("ground_chanqiu_failed.ccbi", 0);
      return false;
     }
     else if (v >= g_RandomParam_2)
    {
      PlayAnimation("ground_chanqiu_randomball.ccbi", 0);
      return false;
    }
    else if (v >= g_ReducedParam_2)
    {
      PlayAnimation("ground_chanqiu_reduced.ccbi", 0);
      g_ballSpeed += g_ReducedParam;
      return false;
    }
    else
    {
      PlayAnimation("ground_chanqiu_success.ccbi", 0);
      return true;
    }
    
    case 2 :  //铲球vs盘带
    var v = Rand() % 1000 + ( o1.dribble + o2.ground ) - ( o2.defence + o2.ground ) + g_OrderParam_4;
    if (v >= g_WinParam_1)
    {
      PlayAnimation("ground_chanqiu_failed.ccbi", 0);
        return false;
      }
      else if (v >= g_RandomParam_1)
    {
      PlayAnimation("ground_chanqiu_randomball.ccbi", 0);
      return false;
    }
    else
    {
      PlayAnimation("ground_chanqiu_success.ccbi", 0);
      return true;
    }
    
    case 3 :  //铲球vs二过一
    var v = Rand() % 1000 + ( o1.pass + o2.ground ) - ( o2.defence + o2.ground ) + g_OrderParam_7 + g_ballSpeed;
    if (v >= g_WinParam_2)
    {
      PlayAnimation("ground_chanqiu_failed.ccbi", 0);
      return false;
    }
    else if (v >= g_RandomParam_2)
    {
      PlayAnimation("ground_chanqiu_randomball.ccbi", 0);
      return false;
    }
    else if (v >= g_ReducedParam_2)
    {
      PlayAnimation("ground_chanqiu_reduced.ccbi", 0);
      g_ballSpeed += g_ReducedParam;
      return false;
    }
    else
    {
      PlayAnimation("ground_chanqiu_success.ccbi", 0);
      return true;
    }
  }
}

// 拦截
function InterceptBall(o1, o2)
{
	switch (g_type)
  {
    case 0 :    //拦截vs传球
    var v = Rand() % 1000 + ( o1.pass + o2.ground ) - ( o2.defence + o2.ground ) + g_OrderParam_2 + g_ballSpeed;
    if (v >= g_WinParam_2)
    {
      PlayAnimation("ground_lanjie_failed.ccbi", 0);
      return false;
    }
    else if (v >= g_RandomParam_2)
    {
      PlayAnimation("ground_lanjie_randomball.ccbi", 0);
      return false;
    }
    else if (v >= g_ReducedParam_2)
    {
      PlayAnimation("ground_lanjie_reduced.ccbi", 0);
      g_ballSpeed += g_ReducedParam;
      return false;
    }
    else
    {
      PlayAnimation("ground_lanjie_success.ccbi", 0);
      return true;
    }

    case 1 :    //拦截vs射门
    var v = Rand() % 1000 + ( o1.shoot + o2.ground ) - ( o2.defence + o2.ground ) + g_OrderParam_11 + g_ballSpeed;
    if (v >= g_WinParam_2)
    {
      PlayAnimation("ground_lanjie_failed.ccbi", 0);
      return false;
    }
     else if (v >= g_RandomParam_2)
    {
      PlayAnimation("ground_lanjie_randomball.ccbi", 0);
      return false;
    }
    else if (v >= g_ReducedParam_2)
    {
      PlayAnimation("ground_lanjie_reduced.ccbi", 0);
      g_ballSpeed += g_ReducedParam;
      return false;
    }
    else
    {
      PlayAnimation("ground_lanjie_success.ccbi", 0);
      return true;
    }

    case 2 :    //拦截vs盘带
    {
      var v = Rand() % 1000 + ( o1.dribble + o2.ground ) - ( o2.defence + o2.ground ) + g_OrderParam_5;
      if (v >= g_WinParam_1)
      {
        PlayAnimation("ground_lanjie_failed.ccbi", 0);
        return false;
      }
      else if (v >= g_RandomParam_1)
      {
        PlayAnimation("ground_lanjie_randomball.ccbi", 0);
        return false;
      }
      else
      {
        PlayAnimation("ground_lanjie_success.ccbi", 0);
        return true;
      }
    }

    case 3 :    //拦截vs二过一
    {
      var v = Rand() % 1000 + ( o1.pass + o2.ground ) - ( o2.defence + o2.ground ) + g_OrderParam_8 + g_ballSpeed;
      if (v >= g_WinParam_2)
      {
        PlayAnimation("ground_lanjie_failed.ccbi", 0);
        return false;
      }
      else if (v >= g_RandomParam_2)
      {
        PlayAnimation("ground_lanjie_randomball.ccbi", 0);
        return false;
      }
      else if (v >= g_ReducedParam_2)
      {
        PlayAnimation("ground_lanjie_reduced.ccbi", 0);
        g_ballSpeed += g_ReducedParam;
        return false;
      }
      else
      {
        PlayAnimation("ground_lanjie_success.ccbi", 0);
        return true;
      }
    }

    case 4 :    //空中传球vs拦截
    PlayAnimation ("ground_paowei.ccbi", 0);
    var v = Rand() % 1000 + ( o1.pass + o2.air ) - ( o2.defence + o2.air ) + g_OrderParam_2 + g_ballSpeed;

  }
}

// 封堵
function BlockBall(o1, o2)
{
	switch (g_type)
  {
    case 0 :   //封堵vs传球
    {
      var v = Rand() % 1000 + ( o1.pass + o2.ground ) - ( o2.defence + o2.ground ) + g_OrderParam_3 + g_ballSpeed;
      if (v >= g_WinParam_2)
      {
        PlayAnimation("ground_fengdu_failed.ccbi", 0);
        return false;
      }
      else if (v >= g_RandomParam_2)
      {
        PlayAnimation("ground_fengdu_randomball.ccbi", 0);
        return false;
      }
      else if (v >= g_ReducedParam_2)
      {
        PlayAnimation("ground_fengdu_reduced.ccbi", 0);
        g_ballSpeed += g_ReducedParam;
        return false;
      }
      else
      {
        PlayAnimation("ground_fengdu_success.ccbi", 0);
        return true;
      }
    }
    
    case 1 :  //封堵vs射门
    {
      var v = Rand() % 1000 + ( o1.shoot + o2.ground ) - ( o2.defence + o2.ground ) + g_OrderParam_12 + g_ballSpeed;
      if (v >= g_WinParam_2)
      {
        PlayAnimation("ground_fengdu_failed.ccbi", 0);
        return false;
       }
       else if (v >= g_RandomParam_2)
      {
        PlayAnimation("ground_fengdu_randomball.ccbi", 0);
        return false;
      }
      else if (v >= g_ReducedParam_2)
      {
        PlayAnimation("ground_fengdu_reduced.ccbi", 0);
        g_ballSpeed += g_ReducedParam;
        return false;
      }
      else
      {
        PlayAnimation("ground_fengdu_success.ccbi", 0);
        return true;
      }
    }
    
    case 2 :    //封堵vs盘带
    {
      var v = Rand() % 1000 + ( o1.dribble + o2.ground ) - ( o2.defence + o2.ground ) + g_OrderParam_6;
      if (v >= g_WinParam_1)
      {
        PlayAnimation("ground_fengdu_failed.ccbi", 0);
        return false;
      }
      else if (v >= g_RandomParam_1)
      {
        PlayAnimation("ground_fengdu_randomball.ccbi", 0);
        return false;
      }
      else
      {
        PlayAnimation("ground_fengdu_success.ccbi", 0);
        return true;
      }
    }
    
    case 3 :     //封堵vs二过一
    {
      var v = Rand() % 1000 + ( o1.pass + o2.ground ) - ( o2.defence + o2.ground ) + g_OrderParam_9 + g_ballSpeed;
      if (v >= g_WinParam_2)
      {
        PlayAnimation("ground_fengdu_failed.ccbi", 0);
        return false;
      }
      else if (v >= g_RandomParam_2)
      {
        PlayAnimation("ground_fengdu_randomball.ccbi", 0);
        return false;
      }
      else if (v >= g_ReducedParam_2)
      {
        PlayAnimation("ground_fengdu_reduced.ccbi", 0);
        g_ballSpeed += g_ReducedParam;
        return false;
      }
      else
      {
        PlayAnimation("ground_fengdu_success.ccbi", 0);
        return true;
      }
    }
  }
}

//获得球
function ReceiveBall(o1)
{
  if (g_isAir)
  {
    PlayAnimation("air_tingqiu.ccbi", 0);
  }
  else
  {
    PlayAnimation("ground_tingqiu.ccbi", 0);
  }
	return;
}
