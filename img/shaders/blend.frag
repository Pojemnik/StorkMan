uniform sampler2D texture;
uniform sampler2D light;

void main()
{
	vec2 pos_flip = vec2(gl_TexCoord[0].x, 1.0 - gl_TexCoord[0].y);
	gl_FragColor = texture2D(texture, gl_TexCoord[0].xy) * texture2D(light, pos_flip);
}