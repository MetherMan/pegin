"""Bake the editable viewer's material shader into legacy game albedo pixels.

The model and original images stay editable. This implements the same linear
RGB mask / HSV operations used by color_controls.js, without changing UVs.
"""
import colorsys
import numpy as np
def linear_rgb(hex_color):
 c=np.array([int(hex_color[i:i+2],16)/255 for i in (1,3,5)])
 return np.where(c<=.04045,c/12.92,((c+.055)/1.055)**2.4)
def smoothstep(a,b,x):
 t=np.clip((x-a)/(b-a),0,1);return t*t*(3-2*t)
def rgb_to_hsv(rgb):
 r,g,b=np.moveaxis(rgb,-1,0);v=np.max(rgb,axis=-1);lo=np.min(rgb,axis=-1);d=v-lo;safe=np.where(d>1e-10,d,1);h=np.zeros_like(v)
 h=np.where(v==b,(r-g)/safe+4,h);h=np.where(v==g,(b-r)/safe+2,h);h=np.where(v==r,(g-b)/safe,h);h=np.where(d>1e-10,(h/6)%1,0)
 return np.stack([h,np.where(v>1e-10,d/np.maximum(v,1e-10),0),v],axis=-1)
def hsv_to_rgb(hsv):
 h,s,v=np.moveaxis(hsv,-1,0);p=np.abs(((h[...,None]+np.array([0,2/3,1/3]))%1)*6-3)
 return v[...,None]*((1-s[...,None])+s[...,None]*np.clip(p-1,0,1))
def uniforms(settings):
 ref=linear_rgb('#246aff');chosen=linear_rgb(settings['blueColor']);rh,rs,rv=colorsys.rgb_to_hsv(*ref);h,s,v=colorsys.rgb_to_hsv(*chosen)
 return h-rh,settings['blueSaturation']*s/rs,settings['blueBrightness']*v/rv,linear_rgb(settings['metalColor'])
def apply_appearance(src,settings):
 hue,saturation,brightness,metal=uniforms(settings);hsv=rgb_to_hsv(src)
 blue=smoothstep(.03,.22,(src[...,2]-np.maximum(src[...,0],src[...,1]))/np.maximum(hsv[...,2],.0001))*smoothstep(.08,.30,hsv[...,1])
 edited=hsv.copy();edited[...,0]=(edited[...,0]+hue+1)%1;edited[...,1]=np.clip(edited[...,1]*saturation,0,1);edited[...,2]*=brightness
 changed=hsv_to_rgb(edited);mask=(1-blue)*smoothstep(.012,.14,np.max(src,axis=-1))
 return (src*(1-blue[...,None])+changed*blue[...,None])*((1-mask[...,None])+metal*mask[...,None])
