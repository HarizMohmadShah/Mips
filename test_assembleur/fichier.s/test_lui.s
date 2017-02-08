.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
lui $28, 0x1234
lui $28, 0xABCD
# max_cycle 50
# pout_start
# 12340000
# ABCD0000
# pout_end

end:                # The End!

