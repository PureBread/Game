#version 150

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texFramebuffer;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

uniform float time = 0;
uniform float mag = 0.5;

vec2 noiseEdges(vec2 uv){
	float amount = 0.9;
	vec2 olduv = uv;
	uv -= vec2(0.5, 0.5);
	float mag = max(0, -amount + length(abs(uv)*2));
	uv.x = rand(uv*time);
	uv.y = rand(uv*time);
	return olduv + (uv - olduv)*mag;
}

vec4 stuff(vec2 uv){
	vec4 res = texture(texFramebuffer, Texcoord);
	res += texture(texFramebuffer, Texcoord + rand(vec2(Texcoord.x, time))*vec2(0.01,0.01));
	res += texture(texFramebuffer, Texcoord + rand(vec2(Texcoord.x, time))*vec2(-0.01,0.01));
	res += texture(texFramebuffer, Texcoord + rand(vec2(Texcoord.x, time))*vec2(-0.01,-0.01));
	res += texture(texFramebuffer, Texcoord + rand(vec2(Texcoord.x, time))*vec2(0.01,-0.01));

	res += texture(texFramebuffer, Texcoord + rand(vec2(Texcoord.x, time))*vec2(0.005,0.005))*2;
	res += texture(texFramebuffer, Texcoord + rand(vec2(Texcoord.x, time))*vec2(-0.005,0.005))*2;
	res += texture(texFramebuffer, Texcoord + rand(vec2(Texcoord.x, time))*vec2(-0.005,-0.005))*2;
	res += texture(texFramebuffer, Texcoord + rand(vec2(Texcoord.x, time))*vec2(0.005,-0.005))*2;

	res += texture(texFramebuffer, Texcoord + rand(vec2(Texcoord.x, time))*vec2(0.0025,0.0025))*3;
	res += texture(texFramebuffer, Texcoord + rand(vec2(Texcoord.x, time))*vec2(-0.0025,0.0025))*3;
	res += texture(texFramebuffer, Texcoord + rand(vec2(Texcoord.x, time))*vec2(-0.0025,-0.0025))*3;
	res += texture(texFramebuffer, Texcoord + rand(vec2(Texcoord.x, time))*vec2(0.0025,-0.0025))*3;

	return res/30;
}

vec4 stuff2(vec2 uv){
	vec4 c = texture(texFramebuffer, uv);
	//if(c.a <= 0.5){
		uv.x += (uv.x - rand(vec2(time,time)))*(c.x);
		uv.y += (uv.y - rand(vec2(time,time)))*(c.y);
	//}
	return texture(texFramebuffer, uv);
}


float map(float val, float min1, float max1, float min2, float max2){
	return (((val - min1) * (max2 - min2)) / (max1 - min1)) + min2;
}

vec4 stuff3(vec2 uv){
	vec4 res = texture(texFramebuffer, Texcoord);

	float d = (Texcoord.y - 0.5);

	res += texture(texFramebuffer, Texcoord + d*vec2(0.01,0.01));
	res += texture(texFramebuffer, Texcoord + d*vec2(-0.01,0.01));
	res += texture(texFramebuffer, Texcoord + d*vec2(-0.01,-0.01));
	res += texture(texFramebuffer, Texcoord + d*vec2(0.01,-0.01));

	res += texture(texFramebuffer, Texcoord + d*vec2(0.005,0.005))*2;
	res += texture(texFramebuffer, Texcoord + d*vec2(-0.005,0.005))*2;
	res += texture(texFramebuffer, Texcoord + d*vec2(-0.005,-0.005))*2;
	res += texture(texFramebuffer, Texcoord + d*vec2(0.005,-0.005))*2;

	res += texture(texFramebuffer, Texcoord + d*vec2(0.0025,0.0025))*3;
	res += texture(texFramebuffer, Texcoord + d*vec2(-0.0025,0.0025))*3;
	res += texture(texFramebuffer, Texcoord + d*vec2(-0.0025,-0.0025))*3;
	res += texture(texFramebuffer, Texcoord + d*vec2(0.0025,-0.0025))*3;

	return res/30;
}

vec4 stuff4(vec2 uv){
	float xd = (0.75 - abs(uv.x-0.5)-0.25);
	float yd = (0.75 - abs(uv.y-0.5)-0.25);

	uv.x += sin(time*0.1001f)*0.1f*xd;
	uv.y += sin(time*0.0501f)*0.1f*yd;


	vec4 res = texture(texFramebuffer, uv)*4;

	float d = (0.5 - uv.y)*0.1;

	uv.y = (uv.y - (0.1f*sin(time))*d);
	//uv.y += rand(vec2(time,uv.x))*d;
	res += texture(texFramebuffer, uv)*3;

	uv.y = (uv.y - (0.1f*sin(time))*d);
	//uv.y += rand(vec2(time,uv.x))*d;
	res += texture(texFramebuffer, uv)*2;

	uv.y = (uv.y - (0.1f*sin(time))*d);
	//uv.y += rand(vec2(time,uv.x))*d;
	res += texture(texFramebuffer, uv);


	return res/10;
}

void main() {
	vec2 v = vec2(Texcoord.x, rand(Texcoord));
	float d = distance(Texcoord, vec2(0.5));

    //outColor = vec4(texture(texFramebuffer, noiseEdges(Texcoord)));

    //outColor = vec4(stuff(Texcoord));
    //outColor = vec4(stuff2(Texcoord));
    //outColor = vec4(stuff3(Texcoord));
    outColor = vec4(stuff4(Texcoord));
}
