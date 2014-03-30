#!/usr/bin/env node

var fs = require('fs');
var cp = require('child_process');
var assert= require('assert');

testFiles = ['../FootBallX/JS/CompactFunctions.js'];

var rand_num = 0;
var expectAnimation = [];

function Log(s)
{
	console.log(s);
}

function SetRand(num)
{
	rand_num = num;
}

function Rand()
{
	return rand_num;
}


function PlayAnimation(ccbi_name, delay)
{
	expectAnimation.push(ccbi_name);
}

function CheckExpectAnimation(s)
{
	for (e in expectAnimation)
	{
		if (expectAnimation[e] === s)
		{
			return true;
		}
	}
	
	Log("Expect list:");
	for (e in expectAnimation)
	{
		Log(expectAnimation[e]);
	}
	return false;
}


function main()
{
	var arguments = process.argv.splice(2);

	if (arguments[0] == 1)
	{
		do_test();
	}
	else
	{
		build_test();
	}
}


function build_test()
{
	Log("build test start...");
	
	for (i in testFiles)
	{
		var testTarget = fs.readFileSync(testFiles[i], "utf8");
		var testCode = fs.readFileSync(__filename, "utf8");
		var caseCode = fs.readFileSync('case.js', 'utf8');

		var idx = testCode.lastIndexOf("main();");
		testCode = testCode.slice(0, idx);

		var output = testCode + testTarget;
		output += caseCode;
		output += "main();";

		var testFilename = __dirname + '/test' + i + '.js';
		fs.writeFileSync(testFilename, output);
	
		var testProc = cp.spawn('node', [testFilename, '1']);
	
		testProc.stdout.setEncoding('utf8');
		testProc.stdout.on('data', function(data){
			Log(data);
		});
		
		testProc.stderr.setEncoding('utf8');
		testProc.stderr.on('data', function(data){
			console.error(data);
		});
	}
	Log("build test done!");
}


function do_test()
{
	Log('----------------------->');
	for (c in test_case)
	{
		Log(test_case[c].name);
		expectAnimation = [];
		test_case[c].func();
		Log('\n');
	}
	Log('<-----------------------');
}



main();

////////////////////////////////////////////////////////////
