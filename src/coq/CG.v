Require Import GappaDefs.

Require Import Interval.Tactic.
Require Import Coquelicot.Coquelicot.

Open Scope R_scope.

Lemma assoc_rnd_bound :
  forall a_ b_ c_ a b c : R,
  a = rnd64ne a_ ->
  b = rnd64ne b_ ->
  c = rnd64ne c_ ->
  0 <= a <= 10 ->
  0 <= b <= 10 ->
  0 <= c <= 10 ->
  abs (rnd64ne (rnd64ne (a + b) + c) - rnd64ne (a + rnd64ne (b + c))) < 1.
Proof.
  intros.
  unfold rnd64ne.
  Admitted.

Lemma assoc_bound :
  forall a_ b_ c_ a b c : R,
  a = rnd64ne a_ ->
  b = rnd64ne b_ ->
  c = rnd64ne c_ ->
  0 <= a <= 1000 ->
  0 <= b <= 1000 ->
  rnd64ne ((a + b) + c) =  rnd64ne (a + (b + c) ).
Proof.
  intros.
  unfold rnd64ne.
  rewrite -> Rplus_assoc.
  reflexivity.
Qed.
