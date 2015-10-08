```python
1 Load cascade classifiers trained
2 Open Web Cam stream
3 while true:
4   read frame from stream
5   convert frame to grey
6   if eye template has been built:
7     track eyes using the eye template
8   else:
9     detect eyes with Viola Johns method and build eye template
11  save eye position
12  draw grid
13  draw rectangle for enclosing eye
14  generate direction command using eye position
15  send command to Eye bedded system
```
