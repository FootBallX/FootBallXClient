#!/usr/bin/env node

var fs = require('fs');

var listFilename = './animList.txt';
// var listFilename = './test.txt';
var serverOutput = '../../FootballXServer/game-server/app/shared/aniDefs.js';
var clientOutput = '../FootBallX/Classes/aniDefs.h';


var toObject = function(a) {
	var o = {};
	for (var i = 0, j = a.length; i < j; i = i + 1) { // 这里我调整了下, YUI源码中是i<a.length 
		o[a[i]] = true;
	}
	return o;
};

var keys = function(o) {
	var a = [],
		i;
	for (i in o) {
		if (o.hasOwnProperty(i)) { // 这里, YUI源码中是lang.hasOwnProperty(o, i) 
			a.push(i);
		}
	}
	return a;
};

var uniq = function(a) {
	return keys(toObject(a));
};


Array.prototype.remove=function(dx) 
{ 
    if(isNaN(dx)||dx>this.length){return false;} 
    for(var i=0,n=0;i<this.length;i++) 
    { 
        if(this[i]!=this[dx]) 
        { 
            this[n++]=this[i] 
        } 
    } 
    this.length-=1 
}

Array.prototype.findAndRemove = function(o)
{
	for (var i = 0; i < this.length; ++i)
	{
		if (this[i] === o)
		{
			this.remove(i);
			--i;
		}
	}
}


var serv = function() {
	var buf = fs.readFileSync(listFilename, "utf8");
	buf = buf.replace(/.ccb/g, '_ccbi');
	buf = buf.replace(/ /g, '');
	
	var aniList = buf.split(/\r?\n/ig);
	aniList = uniq(aniList);
	
	aniList.findAndRemove('');
	aniList.findAndRemove('\n');
	aniList.findAndRemove('\r');
	
	var outBuf = 'module.exports = {\n\tAnimations: {';
	
	for (var i = 0; i < aniList.length; ++i)
	{
		var line = aniList[i];
	
		if (i > 0)
		{
			outBuf += ',';
		}
		outBuf += '\n\t\t' + line + ' : ' + i;
	}
	
	outBuf += '\n\t}\n\}';
	
	fs.writeFileSync(serverOutput, outBuf, 'utf8');
}


var client = function() {
	var buf = fs.readFileSync(listFilename, "utf8");
	buf = buf.replace(/.ccb/g, '.ccbi');
	buf = buf.replace(/ /g, '');
	
	var aniList = buf.split(/\r?\n/ig);
	aniList = uniq(aniList);
	
	aniList.findAndRemove('');
	aniList.findAndRemove('\n');
	aniList.findAndRemove('\r');

	outBuf = 
	'#ifndef __AniDefs_h__\n' +
	'#define __AniDefs_h__\n\n' +
	'static const std::vector<std::string> g_aniNames = {\n';
	
	for (var i = 0; i < aniList.length; ++i)
	{
		var line = aniList[i];
		outBuf += '\t"' + line + '",\t\t//' + i + '\n';
	}
	
	outBuf += 
	'};\n\n' +
	'#endif // __AniDefs_h__\n';
	
	fs.writeFileSync(clientOutput, outBuf, 'utf8');
}

var main = function() {
	// for server
	serv();
	
	// for client
	client();

}


main();