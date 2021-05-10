Require Export Reals.
Require Export Gappa.Gappa_tactic.
From Flocq Require Export Core. (* Equivalently Require Export Flocq.Core.Core *)
(* Other Flocq modules: Binary Bits Plus_error  *)
Open Scope R_scope.

(** Specify the rounding mode corresponding to IEEE-754's binary64
    "round to nearest, ties break even" format. [rounding_float] is the
    same as [round], but uses some Gappa definitions. *)
Definition rnd64zr := rounding_float rndZR 53 (-1074).
Definition rnd64ne := rounding_float rndNE 53 (-1074).
(** Describe the IEEE-754 binary64 format. A real number x is
    representable as a double if you can prove binary64 x. *)
Definition binary64 :=
  generic_format radix2 (FLT_exp (-1074) 53).
(** Machine epsilon for binary64 - the maximum relative error for
    an operation to be "correctly rounded." *)
Definition eps64 := powerRZ 2 (-53). (* 1/9007199254740992 *)

(** Relative error. We define relative error at 0 to be just 0. *)
Definition rel_err (x_hat x : R) : R :=
  if (Req_bool R0 x)
  then R0
  else Rabs ((x_hat - x) / x).

(** This is a test of the Gappa executable. If it fails, make sure
    you have gappa in your path. *)
Example test_gappa :
  forall x y : R,
  3/4 <= x <= 3 ->
  0 <= sqrt x <= 1775 * (powerRZ 2 (-10)).
Proof.
  gappa.
Defined.

Example test_rnd64 :
  forall a_ b_ a b : R,
  a = rnd64ne a_ ->
  b = rnd64ne b_ ->
  52 / 16 <= a <= 53 / 16 ->
  22 / 16 <= b <= 30 / 16 ->
  rnd64ne (a - b) = (a - b).
Proof.
  unfold rnd64ne; gappa.
Defined.

Example test_binary64 :
  forall a b : R,
  binary64 a -> binary64 b ->
  52 / 16 <= a <= 53 / 16 ->
  22 / 16 <= b <= 30 / 16 ->
  binary64 (a - b).
Proof.
  intros a b Ha Hb Ia Ib.
  refine (sym_eq (_ : rnd64zr (a - b) = a - b)).
  revert Ia Ib.
  replace a with (rnd64zr a).
  replace b with (rnd64zr b).
  unfold rnd64zr ; gappa.
Defined.

Lemma scalar_axpy :
  forall a x y : R,
    0 <= a <= 1 ->
    0 <= x <= 1 ->
    0 <= y <= 1 ->
    0 <= a * x + y <= 2.
Proof.
  intros a x y Ia Ix Iy.
  gappa.
Defined.

(** axpy, scalar, with and without fused-multiply-add *)
Lemma scalar_axpy_fma_rnd64 : forall a x y : R,
    binary64 a ->
    binary64 x ->
    binary64 y ->
    0 <= a <= 1 ->
    0 <= x <= 1 ->
    0 <= y <= 1 ->
    0 <= Rabs (rnd64ne (a * x + y) - (a * x + y)) <= eps64.
Proof.
  intros a x y Fa Fx Fy Ia Ix Iy.
  unfold rnd64ne.
  unfold eps64.
  gappa.
Defined.

Lemma scalar_axpy_rnd64 : forall a x y : R,
    binary64 a ->
    binary64 x ->
    binary64 y ->
    -1 <= a <= 1 ->
    -1 <= x <= 1 ->
    -1 <= y <= 1 ->
    0 <= Rabs (rnd64ne (rnd64ne (a * x) + y) - (a * x + y)) <= 2*eps64.
Proof.
  intros a x y Fa Fx Fy Ia Ix Iy.
  unfold rnd64ne.
  unfold eps64.
  gappa.
Defined.

(** Req_bool is a function defined in Flocq, we're just proving
    the converses which don't exist. *)
Lemma Req_false_bool : forall x y : R,
    Req_bool x y = false -> x <> y.
Proof.
  intros x y H.
  pose proof (Req_bool_spec x y) as Hp.
  induction Hp.
  - inversion H.
  - assumption.
Defined.

Lemma Req_true_bool : forall x y : R,
    Req_bool x y = true -> x = y.
Proof.
  intros x y H.
  pose proof (Req_bool_spec x y) as Hp.
  induction Hp.
  - assumption.
  - inversion H.
Defined.

(** It is often convenient to represent theorems with relative
    error. However, rel_error is more complex than absolute error
    since it is defined piecewise (for nonzero and zero) *)
Lemma scalar_axpy_rnd64_rel : forall a x y : R,
    binary64 a ->
    binary64 x ->
    binary64 y ->
    1/2 <= a <= 1 ->
    1/2 <= x <= 1 ->
    1/2 <= y <= 1 ->
    0 <= rel_err (rnd64ne (rnd64ne (a * x) + y)) (a * x + y) <= 2*eps64.
Proof.
  intros a x y Fa Fx Fy Ia Ix Iy.
  unfold rnd64ne.
  unfold eps64.
  unfold rel_err.
  destruct (Req_bool R0 (a * x + y)) eqn:Hre.
  - gappa.
  - apply Req_false_bool in Hre.
    gappa.
Defined.
