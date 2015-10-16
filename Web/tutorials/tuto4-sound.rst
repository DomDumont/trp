.. _tuto4-sound:

====================
Your First Sound
====================


.. code-block:: cpp

    class SoundScene:Scene
        {


        void Init()
            {
            mySFX.Load("sounds/sheep.ogg");  
            mySFX.Play();             
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
            
        Sound  mySFX;
        }



Of course, this is a minimal sample, you can do a lot more with sounds, 
Please take a look at the reference documentation for sound

[Sound](#class-sound)

