

void
    onUndefined (
        void
)
{
    printf("UNDEFINED EXCEPTION!\n");
loop_for_ever:
    goto loop_for_ever;

}

void
    onPrefetchAbort (
        void
)
{
    printf("PREFETCH ABORT EXCEPTION\n");
loop_for_ever:
    goto loop_for_ever;
}

void
    onDataAbort (
        void
)
{
    printf("DATA ABORT EXCEPTION\n");
loop_for_ever:
    goto loop_for_ever;

}

void
    onIRQ (
        void
)
{
    printf("IRQ EXCEPTION\n");
loop_for_ever:
    goto loop_for_ever;
}

void
    onFIQ (
        void
)
{
    printf("FIQ EXCEPTION\n");
loop_for_ever:
    goto loop_for_ever;
}

