uniform sampler2D light;

void main()
{
	vec4 l = texture2D(light, gl_TexCoord[0].xy);
	if(l == vec4(0,0,0,0))
	{
		gl_FragColor = vec4(1,1,1,1);
	}
	else
	{
		gl_FragColor = l;
	}
}