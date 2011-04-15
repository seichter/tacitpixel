
attribute vec4 a_vertex;
attribute vec4 a_normal;
attribute vec2 a_texture_coord;

uniform vec4 u_ambient;
uniform vec4 u_diffuse;
uniform vec4 u_light_pos;
        
uniform mat4 u_projection_mat;               
uniform mat4 u_modelview_mat;
uniform vec4 u_viewer_pos;

varying vec4 v_normal;
varying vec4 v_view_vec;
varying vec2 v_texture_coord;
              
void main()                          
{

	gl_Position = u_projection_mat * u_modelview_mat * a_vertex;   

	v_normal = a_normal;
	v_view_vec = u_viewer_pos - a_vertex;       	
	v_texture_coord = a_texture_coord;

}                                   
