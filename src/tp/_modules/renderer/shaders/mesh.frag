
varying vec4 v_normal;
varying vec4 v_view_vec;
varying vec2 v_texture_coord;

uniform vec4 u_colour;
uniform sampler2D u_texture;

void main()                               
{    
	float v = 0.5 * (1.0 + dot(normalize(v_view_vec), v_normal)); 
	//gl_FragColor = v * u_colour;
	gl_FragColor = texture2D(u_texture, v_texture_coord);
	//gl_FragColor = vec4(0.8,0.8,0.8,1.0);
}
