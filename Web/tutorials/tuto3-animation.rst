.. _tuto3-animation:

====================
Your First Animation
====================

Concerning animations, T.R.P. uses sequences of images.
Again, T.R.P. gets all images from a spritesheet.

In order to play an animation you must

1. Export your animation as a sequence of images
2. Build a spritesheet with all your images (again use shoebox for this)


You can use any nomenclature you want, the only constraint is to start at frame 0 and end at nbframe-1;
The magic then append in the Load function where you can explain T.R.P. the naming you want to use.
Let me try to explain this better throught an example :


Let's say you have a ten frames animation called horse_run.

you can use any naming and any image format you want for your frames for example horse-001.png Horse\_00001.jpg
but you must tell T.R.P. how your namaing works.

For example, if you want to call each frame like this : horse-000.png ... horse-009.png

then the "FORMAT" will be "horse","%s-%03d.png"


some other samples :

for Horse\_00000.jpg to Horse\_00009.jpg the format will be "horse","%s\_%05d.jpg"

Load(myAtlas,"cocci","%s_%05d.png",10);


The minimal sample you can have to play an animation is this one

~~~~ { .Cpp }

    class AnimationScene:Scene
        {
        Animation myAnim;
        Atlas  myAtlas;

        void Init()
            {
            myAtlas.Load("graphics/broute");                 
            myAnim.Load(myAtlas,"broute","%s_%05d.tga",30);
            myAnim.SetFPS(12);
            myAnim.Play(E_MODE_NORMAL,C_INFINITE_LOOP);
            }
            
        void OnUpdate(uint64 _delta)
            {   
            myAnim.Update(_delta);
            }

        void OnRender(uint64 _delta)
            {
            WND_ClearWithColor(80,80,80,255);
            myAnim.Render();
            }

        void OnShutdown()
            {
            myAtlas.UnLoad();
            myAnim.UnLoad();
            }
            
        }

~~~~

Of course, this is a minimal sample, you can do a lot more with animations, 
Please take a look at the reference documentation for animations

[Animation](#class-animation)

