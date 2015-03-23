


## Your First Music


~~~~ { .Cpp }

    class MusicScene:Scene
        {


        void Init()
            {
            myMusic.Load("sounds/invitation.mod");  
            myMusic.Play();             
            }
            
        void OnUpdate(uint64 _delta)
            {   
            
            }

        void OnRender(uint64 _delta)
            {
            WND_ClearWithColor(80,80,80,255);
            }

        void OnShutdown()
            {
            }
            
        Music  myMusic;
        }

~~~~

Of course, this is a minimal sample, you can do a lot more with music, 
Please take a look at the reference documentation for music

[Music](#class-music)

