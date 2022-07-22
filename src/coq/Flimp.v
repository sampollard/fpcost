(* Flimp - Floating point imperative programs *)

Require Export Reals.

Open Scope R_scope.

(* Precision for floating point; that is, for all op *)
Module Type Precision.
  Parameter eps : R.
  Axiom epsilon_gt_zero : eps > 0.
End Precision.

Record Err : Type := mkdistr {
  mean : R;
  variance : R
}.

(* A really is a real plus an e (error) *)
Record really : Type := mkreally {
  r: R;
  e: Err
}.
