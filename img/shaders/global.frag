uniform sampler2D texture;
uniform sampler2D light;

void main()
{
	vec4 t = texture2D(texture, gl_TexCoord[0].xy);
	vec4 l = texture2D(light, gl_TexCoord[0].xy);
	if(l == vec4(0,0,0,0))
	{
		gl_FragColor = l;
	}
	else
	{
		gl_FragColor = l;
	}
}