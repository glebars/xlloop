/*******************************************************************************
 * This program and the accompanying materials
 * are made available under the terms of the Common Public License v1.0
 * which accompanies this distribution, and is available at 
 * http://www.eclipse.org/legal/cpl-v10.html
 * 
 * Contributors:
 *     Peter Smith
 *******************************************************************************/
package org.boris.expr;

public class ExprMultiplication extends ExprEvaluatable
{
    private Expr rhs;
    private Expr lhs;

    public ExprMultiplication(Expr lhs, Expr rhs) {
        super(ExprType.Multiplication);
        this.lhs = lhs;
        this.rhs = rhs;
    }

    public Expr getLHS() {
        return lhs;
    }

    public void setLHS(Expr lhs) {
        this.lhs = lhs;
    }

    public Expr getRHS() {
        return rhs;
    }

    public void setRHS(Expr rhs) {
        this.rhs = rhs;
    }

    public Expr evaluate() throws ExprException {
        return new ExprDouble(((ExprNumber) lhs).doubleValue() *
                ((ExprNumber) rhs).doubleValue());
    }
}