workflow "CI" {
  on = "push"
  resolves = ["Checkout"]
}

action "Checkout" {
  uses = "actions/checkout@v1"
}
