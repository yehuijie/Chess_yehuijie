Gentili Professori,
io non posseggo un calcolatore che mi permetta di far girare UE5 e per questo ho lavorato sul progetto in Laboratorio.
Purtroppo, il 30 aprile, ultimo giorno utile per lavorare al progetto nel Lab per la prima data di consegna, non sono riuscito a rivedere il progetto e per questo ci sono delle porzioni di codice in cui funzioni o classi non vengono utilizzati, ma che non ho voluto cancellare per timore che poi il progetto non compilasse più.
Lascio qui sotto le funzioni/classi in questione (ho commentato specificando questo problema anche nel codice stesso)

1. BasePiece.h --> void SelfDestroy();
2. BlackPlayer in toto
3. GameInstance.h -> quasi tutto
4. GameMode.h -> bool IsGameOver;
              -> int32 MoveCounter;
              -> int32 RandIdx;
              -> int32 GetNextPlayer(int32 Player);
	            -> void TurnNextPlayer();
              -> void MoveBlackPiece();
              -> void SetBlackToMove();
              -> int32 GetBlackArraySize();
5. Chess_HumanPlayer -> void OnWin();
                     -> void OnLose();
6. Chess_PlayerInterface -> enum class EPiece1 : uint8
7. Chess_RandomPlayer in toto
8. GameField.h -> int32 WinSize;
