import { ArgPass } from "./argument"
import { GetOptionDesc } from "./option"

/**
 * 短选项
 */
export function ShortOpt(opt: string) {
    return (target: any, propertyKey: string) => {
        GetOptionDesc(target, propertyKey).short = opt
    }
}

/**
 * 长选项
 */
export function LongOpt(opt: string) {
    return (target: any, propertyKey: string) => {
        GetOptionDesc(target, propertyKey).long = opt
    }
}

/**
 * 选项描述
 */
export function Brief(brief: string) {
    return (target: any, propertyKey: string) => {
        GetOptionDesc(target, propertyKey).brief = brief
    }
}

/**
 * 选项参数列表或参数检查函数
 */
export function Args(args: string[] | ArgPass) {
    return (target: any, propertyKey: string) => {
        GetOptionDesc(target, propertyKey).arg = args
    }
}

/**
 * 选项参数补全函数
 */
export function Complete(complete: ((arg?:string) => string[] | Promise<string[]>)) {
    return (target: any, propertyKey: string) => {
        GetOptionDesc(target, propertyKey).complete = complete
    }
}

/**
 * 必要选项
 */
export function Required() {
    return (target: any, propertyKey: string) => {
        GetOptionDesc(target, propertyKey).required = true
    }
}

/**
 * 可重复选项
 */
export function Repeat() {
    return (target: any, propertyKey: string) => {
        GetOptionDesc(target, propertyKey).repeat = true
    }
}