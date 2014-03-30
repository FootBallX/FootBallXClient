
var test_case = [
{'name':'PassBall_ground',
'func':function()
	{
		var o = {};
		SetRand(0);
		g_isAir = false;
		StartPassBall(o, true);
		assert.ok(CheckExpectAnimation("ground_chuanqiu.ccbi"), 'ground_chuanqiu.ccbi');
	}
},

{'name':'PassBall_air',
'func':function()
	{
		var o = {};
		SetRand(0);
		g_isAir = true;
		StartPassBall(o, true);
		assert.ok(CheckExpectAnimation("air_chuanqiu.ccbi"), 'air_chuanqiu.ccbi');
	}
},










{'name':'TackleVsPass_failed',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!TackleBall(o1, o2), 'TackleVsPass_failed');
		assert.ok(CheckExpectAnimation("ground_chanqiu_failed.ccbi"), 'ground_chanqiu_failed.ccbi');
	}
},

{'name':'TackleVsPass_randomball',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":500, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!TackleBall(o1, o2), 'TackleVsPass_randomball');
		assert.ok(CheckExpectAnimation("ground_chanqiu_randomball.ccbi"), 'ground_chanqiu_randomball.ccbi');
	}
},

{'name':'TackleVsPass_reduced',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":800, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!TackleBall(o1, o2), 'TackleVsPass_reduced');
		assert.ok(CheckExpectAnimation("ground_chanqiu_reduced.ccbi"), 'ground_chanqiu_reduced.ccbi');
	}
},

{'name':'TackleVsPass_success',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1000, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(TackleBall(o1, o2), 'TackleVsPass_success');
		assert.ok(CheckExpectAnimation("ground_chanqiu_success.ccbi"), 'ground_chanqiu_success.ccbi');
	}
},










{'name':'TackleVsShoot_failed',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "Pass":1, "shoot":500, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "Pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 1;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!TackleBall(o1, o2), 'TackleVsShoot_failed');
		assert.ok(CheckExpectAnimation("ground_chanqiu_failed.ccbi"), 'ground_chanqiu_failed.ccbi');
	}
},

{'name':'TackleVsShoot_randomball',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":500, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":500, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 1;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!TackleBall(o1, o2), 'TackleVsShoot_randomball');
		assert.ok(CheckExpectAnimation("ground_chanqiu_randomball.ccbi"), 'ground_chanqiu_randomball.ccbi');
	}
},

{'name':'TackleVsShoot_reduced',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":500, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":800, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 1;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!TackleBall(o1, o2), 'TackleVsShoot_reduced');
		assert.ok(CheckExpectAnimation("ground_chanqiu_reduced.ccbi"), 'ground_chanqiu_reduced.ccbi');
	}
},

{'name':'TackleVsShoot_success',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":500, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1000, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 1;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(TackleBall(o1, o2), 'TackleVsShoot_success');
		assert.ok(CheckExpectAnimation("ground_chanqiu_success.ccbi"), 'ground_chanqiu_success.ccbi');
	}
},










{'name':'TackleVsDribble_failed',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":500, "Pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "Pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 2;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!TackleBall(o1, o2), 'TackleVsDribble_failed');
		assert.ok(CheckExpectAnimation("ground_chanqiu_failed.ccbi"), 'ground_chanqiu_failed.ccbi');
	}
},

{'name':'TackleVsDribble_randomball',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":500, "pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":500, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 2;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!TackleBall(o1, o2), 'TackleVsDribble_randomball');
		assert.ok(CheckExpectAnimation("ground_chanqiu_randomball.ccbi"), 'ground_chanqiu_randomball.ccbi');
	}
},

{'name':'TackleVsDribble_success',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1000, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 2;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(TackleBall(o1, o2), 'TackleVsDribble_success');
		assert.ok(CheckExpectAnimation("ground_chanqiu_success.ccbi"), 'ground_chanqiu_success.ccbi');
	}
},










{'name':'TackleVsOneTwo_failed',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!TackleBall(o1, o2), 'TackleVsOneTwo_failed');
		assert.ok(CheckExpectAnimation("ground_chanqiu_failed.ccbi"), 'ground_chanqiu_failed.ccbi');
	}
},

{'name':'TackleVsOneTwo_randomball',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":500, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!TackleBall(o1, o2), 'TackleVsOneTwo_randomball');
		assert.ok(CheckExpectAnimation("ground_chanqiu_randomball.ccbi"), 'ground_chanqiu_randomball.ccbi');
	}
},

{'name':'TackleVsOneTwo_reduced',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":800, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!TackleBall(o1, o2), 'TackleVsOneTwo_reduced');
		assert.ok(CheckExpectAnimation("ground_chanqiu_reduced.ccbi"), 'ground_chanqiu_reduced.ccbi');
	}
},

{'name':'TackleVsOneTwo_success',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1000, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(TackleBall(o1, o2), 'TackleVsOneTwo_success');
		assert.ok(CheckExpectAnimation("ground_chanqiu_success.ccbi"), 'ground_chanqiu_success.ccbi');
	}
},










{'name':'InterceptVsPass_failed',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!InterceptBall(o1, o2), 'InterceptVsPass_failed');
		assert.ok(CheckExpectAnimation("ground_lanjie_failed.ccbi"), 'ground_lanjie_failed.ccbi');
	}
},

{'name':'InterceptVsPass_randomball',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":700, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":500, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!InterceptBall(o1, o2), 'InterceptVsPass_randomball');
		assert.ok(CheckExpectAnimation("ground_lanjie_randomball.ccbi"), 'ground_lanjie_randomball.ccbi');
	}
},

{'name':'InterceptVsPass_reduced',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":500, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!InterceptBall(o1, o2), 'InterceptVsPass_reduced');
		assert.ok(CheckExpectAnimation("ground_lanjie_reduced.ccbi"), 'ground_lanjie_reduced.ccbi');
	}
},

{'name':'InterceptVsPass_success',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1000, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(InterceptBall(o1, o2), 'InterceptVsPass_success');
		assert.ok(CheckExpectAnimation("ground_lanjie_success.ccbi"), 'ground_lanjie_success.ccbi');
	}
},










{'name':'InterceptVsShoot_failed',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":500, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 1;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!InterceptBall(o1, o2), 'InterceptVsShoot_failed');
		assert.ok(CheckExpectAnimation("ground_lanjie_failed.ccbi"), 'ground_lanjie_failed.ccbi');
	}
},

{'name':'InterceptVsShoot_randomball',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":700, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":500, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 1;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!InterceptBall(o1, o2), 'InterceptVsShoot_randomball');
		assert.ok(CheckExpectAnimation("ground_lanjie_randomball.ccbi"), 'ground_lanjie_randomball.ccbi');
	}
},

{'name':'InterceptVsShoot_reduced',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":500, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":500, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 1;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!InterceptBall(o1, o2), 'InterceptVsShoot_reduced');
		assert.ok(CheckExpectAnimation("ground_lanjie_reduced.ccbi"), 'ground_lanjie_reduced.ccbi');
	}
},

{'name':'InterceptVsShoot_success',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1000, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 1;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(InterceptBall(o1, o2), 'InterceptVsShoot_success');
		assert.ok(CheckExpectAnimation("ground_lanjie_success.ccbi"), 'ground_lanjie_success.ccbi');
	}
},










{'name':'InterceptVsDribble_failed',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":500, "Pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "Pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 2;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!InterceptBall(o1, o2), 'InterceptVsDribble_failed');
		assert.ok(CheckExpectAnimation("ground_lanjie_failed.ccbi"), 'ground_lanjie_failed.ccbi');
	}
},

{'name':'InterceptVsDribble_randomball',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":500, "pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":500, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 2;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!InterceptBall(o1, o2), 'InterceptVsDribble_randomball');
		assert.ok(CheckExpectAnimation("ground_lanjie_randomball.ccbi"), 'ground_lanjie_randomball.ccbi');
	}
},

{'name':'InterceptVsDribble_success',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1000, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 2;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(InterceptBall(o1, o2), 'InterceptVsDribble_success');
		assert.ok(CheckExpectAnimation("ground_lanjie_success.ccbi"), 'ground_lanjie_success.ccbi');
	}
},










{'name':'InterceptVsOneTwo_failed',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!InterceptBall(o1, o2), 'InterceptVsOneTwo_failed');
		assert.ok(CheckExpectAnimation("ground_lanjie_failed.ccbi"), 'ground_lanjie_failed.ccbi');
	}
},

{'name':'InterceptVsOneTwo_randomball',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":300, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!InterceptBall(o1, o2), 'InterceptVsOneTwo_randomball');
		assert.ok(CheckExpectAnimation("ground_lanjie_randomball.ccbi"), 'ground_lanjie_randomball.ccbi');
	}
},

{'name':'InterceptVsOneTwo_reduced',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":600, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(!InterceptBall(o1, o2), 'InterceptVsOneTwo_reduced');
		assert.ok(CheckExpectAnimation("ground_lanjie_reduced.ccbi"), 'ground_lanjie_reduced.ccbi');
	}
},

{'name':'InterceptVsOneTwo_success',
'func':function()
	{
		var o1 = {"id":"id_o1", "type":1, "strength":1, "speed":1, "dribble":1, "pass":500, "shoot":1, "defence":1, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		var o2 = {"id":"id_o2", "type":2, "strength":1, "speed":1, "dribble":1, "pass":1, "shoot":1, "defence":1000, "take":1, "hit":1, "attack":1, "ground":1, "air":1 };
		g_type = 0;
		g_ballSpeed = 0;
		SetRand(500);
		assert.ok(InterceptBall(o1, o2), 'InterceptVsOneTwo_success');
		assert.ok(CheckExpectAnimation("ground_lanjie_success.ccbi"), 'ground_lanjie_success.ccbi');
	}
},










{'name':'ReceiveBall_ground',
'func':function()
	{
		var o = {};
		SetRand(0);
		g_isAir = false;
		ReceiveBall(o, true);
		assert.ok(CheckExpectAnimation("ground_tingqiu.ccbi"), 'ground_tingqiu.ccbi');
	}
},

{'name':'ReceiveBall_air',
'func':function()
	{
		var o = {};
		SetRand(0);
		g_isAir = true;
		ReceiveBall(o, true);
		assert.ok(CheckExpectAnimation("air_tingqiu.ccbi"), 'air_tingqiu.ccbi');
	}
},
















];

