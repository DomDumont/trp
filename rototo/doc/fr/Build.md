# Build 

1. Verifier qu'il n'y a pas de framework sdl2 qui trainent sur le mac


	sudo find / -name 'SDL2.framework' 2>/dev/null

2. Dans XCode vérifiez que les DerivedData sont bien sur Custom/Legacy

3. Dans SDL choisir la target Static Library.


