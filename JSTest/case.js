
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





];

